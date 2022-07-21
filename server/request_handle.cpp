#include "request_handle.h"
#include "sql_engine.h"
#include "resheader_packet.h"
#include "field_packet.h"
#include "error_packet.h"
#include "eof_packet.h"
#include "row_packet.h"
#include "ok_packet.h"
#include "session_info.h"
#include "buffer.h"
#include "global.h"
#include "server.h"
#include "error.h"
#include "util.h"
#include "row.h"
#include "log.h"
#define BLOCK_SIZE	2048

using namespace CatDB::Server;
using namespace CatDB::Common;
using namespace CatDB::Sql;

RequestHandle::RequestHandle(int fd,ServerService& service)
	:m_fd(fd),
	m_read_cache(service.config().cache_size()),
	m_write_cache(service.config().cache_size()),
	m_server_service(service)
{
	NetService::CallbackFunc func = std::bind(&RequestHandle::notify_socket,this, std::placeholders::_1, std::placeholders::_2);
	if(m_server_service.m_net_service.register_io(m_fd, NetService::E_RW, func) < 0)
	{
		close_connection();
	}
	session_info = SessionInfo::make_session_info();
	session_info->set_session_log_level(service.config().log_level());
	session_info->set_session_log_module(service.config().log_module());
	session_info->set_query_timeout(service.config().query_timeout());
}

RequestHandle::~RequestHandle()
{
	if(m_fd > 0)
		close_connection();
}

void RequestHandle::set_delete_handle(RequestHandle_s& self)
{
	m_self.swap(self);
}

void RequestHandle::set_login_info(const Loginer::LoginInfo & info, int session_id)
{
	login_info = info;
	session_info->set_cur_database(info.db_name_);
	session_info->set_session_id(session_id);
}

void RequestHandle::notify_socket(int fd, NetService::Event e)
{
	if(e == NetService::E_READ)
		read_socket(fd);
	else if(e == NetService::E_WRITE)
		write_socket(fd);
}

void RequestHandle::read_socket(int fd)
{
	int len = BLOCK_SIZE;
	Buffer buffer;
	buffer_calloc(buffer, BLOCK_SIZE);
		
	len = net_read(fd, buffer.data, len);
	LOG_TRACE("RequestHandle::read_socket data", K(m_fd), K(len));
	if (len < 0) {
		close_connection();
		buffer_free(buffer);
		return;
	}
	buffer.length = len;
	m_read_cache.write(buffer);

	bool state = true;
	while(state)
	{
		state = m_read_cache.read_package(buffer);
		if(state)
		{
			handle_request(buffer.data, buffer.length);
		}
	}
	//disconnect
	if(len == 0)
	{
		close_connection();
	}
	buffer_free(buffer);
}

void RequestHandle::write_socket(int fd)
{
	char buf[BLOCK_SIZE];
	Buffer tmp;
	tmp.capacity = BLOCK_SIZE;
	tmp.data = buf;
	while(true)
	{
		tmp.length = BLOCK_SIZE;
		m_write_cache.have_a_look(tmp);
		size_t len = tmp.length;
		len = net_write(fd, buf, len);
	
		tmp.length = len;
		if(len > 0)
		{
			m_write_cache.read(tmp);
			LOG_TRACE("RequestHandle::write_socket data", K(m_fd), K(len));
		}
		if(len != BLOCK_SIZE)
			break;
	}
}

void RequestHandle::close_connection()
{
	LOG_TRACE("RequestHandle close connection", K(m_fd));
	net_close(m_fd);
	m_server_service.m_net_service.unregister_io(m_fd, NetService::E_RW);
	m_server_service.close_connection(session_info->get_session_id());
	RequestHandle_s copy;
	copy.swap(m_self);
	m_fd = -1;
}

u32 RequestHandle::post_packet(Packet & packet, uint8_t seq)
{
	int ret = SUCCESS;
	int32_t pkt_len = 0;
	int64_t len_pos = 0;
	int64_t pos = 0;

	uint64_t size = packet.get_serialize_size() + PACKET_HEADER_SIZE;
	Common::Buffer_s buf = Common::Buffer::make_buffer(size);
	char* buff = NULL;
	buff = reinterpret_cast<char*>(buf->buf);
	pos += 3;
	Util::store_int1(buff, size, seq, pos);
	ret = packet.serialize(buff, size, pos);
	if (SUCCESS != ret)
	{
		LOG_ERR("serialize packet failed", K(ret));
	}
	else
	{
		len_pos = 0;
		// Ð´Èë°üµÄ³¤¶È
		pkt_len = static_cast<int32_t>(pos - PACKET_HEADER_SIZE);
		Util::store_int3(buff, size, pkt_len, len_pos);
		if (!m_write_cache.write_package((const char*)buf->buf, pos))
		{
			LOG_ERR("RequestHandle write_cache full,drop response");
		}
	}
	return ret;
}

u32 RequestHandle::send_ok_packet(u32 affected_rows)
{
	int ret = SUCCESS;
	OKPacket okpacket;
	okpacket.set_affected_rows(affected_rows);
	ret = post_packet(okpacket, ++seq);
	if (SUCCESS != ret)
	{
		LOG_ERR("failed to send ok packet to mysql client", K(ret));
	}
	return ret;
}

u32 RequestHandle::send_error_packet(u32 err_code, const String & msg)
{
	int ret = SUCCESS;
	ErrorPacket epacket;
	epacket.set_message(msg);
	epacket.set_errcode(static_cast<int>( err_code ));
	ret = post_packet(epacket, ++seq);
	if (SUCCESS != ret)
	{
		LOG_ERR("failed to send error packet to mysql client", K(ret));
	}
	return ret;
}

u32 RequestHandle::send_result_set(const ResultSet_s &result_set)
{
	int ret = SUCCESS;
	int64_t buffer_length = 0;
	int64_t buffer_pos = 0;
	u32 MAX_PACKET_LENGTH = 1024000;
	Common::Buffer_s buf = Common::Buffer::make_buffer(MAX_PACKET_LENGTH);
	char *data_buffer = reinterpret_cast<char *>(buf->buf);
	buffer_length = MAX_PACKET_LENGTH;
	if (SUCCESS != (ret = process_resheader_packet(buf, buffer_pos, result_set)))
	{
		LOG_ERR("process resheasder packet failed", K(ret));
	}
	else if (SUCCESS != (ret = process_field_packets(buf, buffer_pos, result_set)))
	{
		LOG_ERR("process field packet failed", K(ret));
	}
	else if (SUCCESS != (ret = process_eof_packets(buf, buffer_pos, result_set)))
	{
		LOG_ERR("process field eof packet failed", K(ret));
	}
	else if (SUCCESS != (ret = process_row_packets(buf, buffer_pos, result_set)))
	{
		LOG_ERR("process row packet failed", K(ret));
	}
	else if (SUCCESS != (ret = process_eof_packets(buf, buffer_pos, result_set)))
	{
		LOG_ERR("process row eof packet failed", K(ret));
	}
	return ret;
}

u32 RequestHandle::process_resheader_packet(Common::Buffer_s & buff, int64_t & buff_pos, const ResultSet_s &result_set)
{
	int ret = SUCCESS;
	ResheaderPacket header;
	header.set_field_count(result_set->get_column_count());
	header.set_seq(static_cast<uint8_t>(seq+1));
	ret = process_single_packet(buff, buff_pos, header);
	if (SUCCESS != ret)
	{
		LOG_ERR("process resheader packet failed", K(ret));
	}
	return ret;
}

u32 RequestHandle::process_field_packets(Common::Buffer_s & buff, int64_t & buff_pos, const ResultSet_s &result_set)
{
	int ret = SUCCESS;
	for (u32 i = 0; i < result_set->get_column_count(); ++i){
		FieldPacket packet(result_set->get_result_title(i));
		packet.set_type(result_set->get_result_type(i));
		packet.set_seq(seq + 1);
		process_single_packet(buff, buff_pos, packet);
	}
	return ret;
}

u32 RequestHandle::process_eof_packets(Common::Buffer_s & buff, int64_t & buff_pos, const ResultSet_s &result_set)
{
	int ret = SUCCESS;
	EofPacket eof;
	eof.set_warning_count(0);
	eof.set_seq(static_cast<uint8_t>(seq + 1));
	eof.set_server_status(0);
	ret = process_single_packet(buff, buff_pos, eof);
	if (SUCCESS != ret)
	{
		LOG_ERR("process eof packet failed", K(ret));
	}
	return ret;
}

u32 RequestHandle::process_row_packets(Common::Buffer_s & buff, int64_t & buff_pos, const ResultSet_s &result_set)
{
	int ret = SUCCESS;
	Row_s row;
	CHECK(result_set->open());
	while (SUCC(result_set->get_next_row(row))) {
		RowPacket packet(row);
		packet.set_seq(seq + 1);
		process_single_packet(buff, buff_pos, packet);
	}
	int temp_ret = ret;
	if (NO_MORE_ROWS == ret) {
		temp_ret = SUCCESS;
	}
	CHECK(result_set->close());
	ret = temp_ret;
	return ret;
}

u32 RequestHandle::process_single_packet(Common::Buffer_s & buff, int64_t & buff_pos, Packet & packet)
{
	return post_packet(packet, ++seq);
	int ret = SUCCESS;
	int sret = SUCCESS;
	ret = packet.encode( reinterpret_cast<char*>(buff->buf), buff->length, buff_pos);
	if (SIZE_OVERFLOW == ret) //buff not enough to hold this packet
	{
		m_write_cache.write_package(reinterpret_cast<char*>(buff->buf), buff_pos);
		//now we can reuse message buffer
		buff_pos = 0;
		ret = packet.encode(reinterpret_cast<char*>(buff->buf), buff->length, buff_pos);
		++seq;
	}
	return ret;
}

u32 RequestHandle::do_not_support()
{
	int ret = SUCCESS;
	ErrorPacket epacket;
	const char * msg ="cmd not support yet.";
	epacket.set_message(msg);
	epacket.set_errcode(ERR_UNEXPECTED);
	ret = post_packet(epacket, ++seq);
	if (SUCCESS != ret)
	{
		LOG_ERR("failed to send error packet to mysql client", K(ret));
	}
	return ret;
}

u32 RequestHandle::do_cmd_query(const String& query)
{
	SET_GTX(session_info);
	SqlEngine_s engine = SqlEngine::make_sql_engine(query);
	u32 ret = engine->handle_query();
	if (FAIL(ret)) {
		ret = send_error_packet(ret, session_info->get_err_msg(ret));
	} else {
		ResultSet_s result_set = engine->get_query_result();
		if (!result_set) {
			ret = ERR_UNEXPECTED;
		} else if (result_set->is_explain_plan()) {
			String plan_info = result_set->get_explain_info();
			ret = (send_ok_packet(plan_info));
		} else if (result_set->need_send_result()) {
			ret = (send_result_set(result_set));
		} else {
			ret = (send_ok_packet(QUERY_CTX->get_affected_rows()));
		}
		if (FAIL(ret)) {
			ret = send_error_packet(ret, session_info->get_err_msg(ret));
		}
	}
	session_info->end_query();
	return ret;
}

u32 RequestHandle::send_ok_packet(const String& explain_info)
{
	int ret = SUCCESS;
	OKPacket okpacket;
	okpacket.set_message(explain_info);
	ret = post_packet(okpacket, ++seq);
	if (SUCCESS != ret)
	{
		LOG_ERR("failed to send ok packet to mysql client", K(ret));
	}
	return ret;
}

void RequestHandle::handle_request(char* buf, size_t len)
{
	enum enum_server_command command;
	command = (enum enum_server_command)(unsigned char)buf[0];
	seq = 0;
	switch (command)
	{
		case COM_INIT_DB:
		{
			String db(buf + 1, len - 1);
			String query = "use "+db;
			session_info->begin_query(DO_QUERY, query);
			Task_type task = std::bind(&RequestHandle::do_cmd_query, this, query);
			m_server_service.workers().append_task(task);
			break;
		}
		case COM_QUERY:
		{
			String query(buf + 1, len - 1);
			LOG_TRACE("handle client command", K(query));
			session_info->begin_query(DO_QUERY, query);
			Task_type task = std::bind(&RequestHandle::do_cmd_query, this, query);
			m_server_service.workers().append_task(task);
			break;
		}
		case COM_PING:
		{
			session_info->begin_query(PING, "");
			send_ok_packet();
			session_info->end_query();
			break;
		}
		case COM_FIELD_LIST:
		{
			session_info->begin_query(DO_CMD, "");
			do_not_support();
			session_info->end_query();
			break;
		}
		default:
		{
			session_info->begin_query(DO_CMD, "");
			do_not_support();
			session_info->end_query();
		}
	}
}

