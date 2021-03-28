#include "request_handle.h"
#include "query_result.h"
#include "sql_driver.h"
#include "resheader_packet.h"
#include "field_packet.h"
#include "error_packet.h"
#include "eof_packet.h"
#include "row_packet.h"
#include "ok_packet.h"
#include "buffer.h"
#include "global.h"
#include "server.h"
#include "error.h"
#include "plan.h"
#include "util.h"
#include "../tpch.h"
#include "row.h"
#include "log.h"
#define BLOCK_SIZE	2048

using namespace CatDB::Server;
using namespace CatDB;
using namespace CatDB::Sql;
using CatDB::SqlDriver;
static bool is_com_field_list = false;

RequestHandle::RequestHandle(int fd,ServerService& service)
	:m_fd(fd),
	m_read_cache(service.config().cache_size()),
	m_write_cache(service.config().cache_size()),
	m_server_service(service),
	cur_database("test")
{
	NetService::CallbackFunc func = std::bind(&RequestHandle::notify_socket,this, std::placeholders::_1, std::placeholders::_2);
	if(m_server_service.m_net_service.register_io(m_fd, NetService::E_RW, func) < 0)
	{
		close_connection();
	}
}

RequestHandle::~RequestHandle()
{
	if(m_fd > 0)
		close_connection();
}

void RequestHandle::set_delete_handle(std::shared_ptr<RequestHandle>& self)
{
	m_self.swap(self);
}

void RequestHandle::set_login_info(const Loginer::LoginInfo & info)
{
	login_info = info;
	cur_database = info.db_name_;
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
	Log(LOG_TRACE, "RequestHandle", "RequestHandle::read_socket fd %d recv %u bytes data",m_fd,len);
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
		if(state=m_read_cache.read_package(buffer))
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
			Log(LOG_TRACE, "RequestHandle", "RequestHandle::write_socket fd %d send %u bytes data",m_fd,len);
		}
		if(len != BLOCK_SIZE)
			break;
	}
}

void RequestHandle::close_connection()
{
	Log(LOG_TRACE, "RequestHandle", "RequestHandle close connection:%d",m_fd);
	net_close(m_fd);
	m_server_service.m_net_service.unregister_io(m_fd, NetService::E_RW);
	m_server_service.close_connection(m_fd);
	std::shared_ptr<RequestHandle> copy;
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
		Log(LOG_ERR, "RequestHandle", "serialize packet failed packet is %p ret is %d", &packet, ret);
	}
	else
	{
		len_pos = 0;
		// Ð´Èë°üµÄ³¤¶È
		pkt_len = static_cast<int32_t>(pos - PACKET_HEADER_SIZE);
		Util::store_int3(buff, size, pkt_len, len_pos);
		if (!m_write_cache.write_package((const char*)buf->buf, pos))
		{
			Log(LOG_ERR, "RequestHandle", "RequestHandle write_cache full,drop response");
		}
	}
	return ret;
}

u32 RequestHandle::send_ok_packet()
{
	int ret = SUCCESS;
	OKPacket okpacket;
	ret = post_packet(okpacket, ++seq);
	if (SUCCESS != ret)
	{
		Log(LOG_ERR, "RequestHandle", "failed to send ok packet to mysql client ret is %d", ret);
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
		Log(LOG_ERR, "RequestHandle", "failed to send error packet to mysql client ret is %d", ret);
	}
	return ret;
}

u32 RequestHandle::send_result_set(const Plan_s& plan)
{
	int ret = SUCCESS;
	int64_t buffer_length = 0;
	int64_t buffer_pos = 0;
	u32 MAX_PACKET_LENGTH = 1024000;
	Common::Buffer_s buf = Common::Buffer::make_buffer(MAX_PACKET_LENGTH);
	char *data_buffer = reinterpret_cast<char *>(buf->buf);
	buffer_length = MAX_PACKET_LENGTH;
	if (is_com_field_list)
	{
		if (SUCCESS != (ret = process_field_packets(buf, buffer_pos, plan)))
        	{
                	Log(LOG_WARN, "RequestHandle", "process row packet failed ret is %d", ret);
        	}
        	else if (SUCCESS != (ret = process_eof_packets(buf, buffer_pos, plan)))
        	{
                	Log(LOG_WARN, "RequestHandle", "process row eof packet failed ret is %d", ret);
        	}
		return ret;
	}
	if (SUCCESS != (ret = process_resheader_packet(buf, buffer_pos, plan)))
	{
		Log(LOG_WARN, "RequestHandle", "process resheasder packet failed ret is %d", ret);
	}
	else if (SUCCESS != (ret = process_field_packets(buf, buffer_pos, plan)))
	{
		Log(LOG_WARN, "RequestHandle", "process field packet failed ret is %d", ret);
	}
	else if (SUCCESS != (ret = process_eof_packets(buf, buffer_pos, plan)))
	{
		Log(LOG_WARN, "RequestHandle", "process field eof packet failed ret is %d", ret);
	}
	else if (SUCCESS != (ret = process_row_packets(buf, buffer_pos, plan)))
	{
		Log(LOG_WARN, "RequestHandle", "process row packet failed ret is %d", ret);
	}
	else if (SUCCESS != (ret = process_eof_packets(buf, buffer_pos, plan)))
	{
		Log(LOG_WARN, "RequestHandle", "process row eof packet failed ret is %d", ret);
	}
	if (SUCCESS == ret)
	{
		Log(LOG_INFO, "RequestHandle",  "send result set to client");
		
	}
	return ret;
}

u32 RequestHandle::process_resheader_packet(Common::Buffer_s & buff, int64_t & buff_pos, const Plan_s& plan)
{
	int ret = SUCCESS;
	ResheaderPacket header;
	if (plan->get_result_title()) {
		header.set_field_count(plan->get_result_title()->get_row_desc().get_column_num());
	}
	else {
		header.set_field_count(0);
	}
	header.set_seq(static_cast<uint8_t>(seq+1));
	ret = process_single_packet(buff, buff_pos, header);
	if (SUCCESS != ret)
	{
		Log(LOG_ERR, "RequestHandle", "process resheader packet failed ret is %d", ret);
	}
	return ret;
}

u32 RequestHandle::process_field_packets(Common::Buffer_s & buff, int64_t & buff_pos, const Plan_s & plan)
{
	int ret = SUCCESS;
	Common::QueryResult* query_result = dynamic_cast<Common::QueryResult*>(plan->get_result().get());
	u32 row_size = 0;
	Row_s row;
	if (query_result) {
		row_size = query_result->size();
	}
	if(row_size){
		query_result->get_row(0, row);
	}
	u32 column_count = 0;
	if (plan->get_result_title()) {
		column_count = plan->get_result_title()->get_row_desc().get_column_num();
	}
	for (u32 i = 0; i < column_count; ++i){
		Object_s cell;
		plan->get_result_title()->get_cell(i, cell);
		FieldPacket packet(cell->to_string());
		if (row) {
			row->get_cell(i, cell);
			if (cell->is_null()) {
				packet.set_type(T_NULL);
			}
			else {
				packet.set_type(cell->get_type());
			}
		}
		packet.set_seq(seq + 1);
		process_single_packet(buff, buff_pos, packet);
	}
	return ret;
}

u32 RequestHandle::process_eof_packets(Common::Buffer_s & buff, int64_t & buff_pos, const Plan_s & plan)
{
	int ret = SUCCESS;
	EofPacket eof;
	eof.set_warning_count(0);
	eof.set_seq(static_cast<uint8_t>(seq + 1));
	eof.set_server_status(0);
	ret = process_single_packet(buff, buff_pos, eof);
	if (SUCCESS != ret)
	{
		Log(LOG_ERR, "RequestHandle", "process eof packet failed ret is %d", ret);
	}
	return ret;
}

u32 RequestHandle::process_row_packets(Common::Buffer_s & buff, int64_t & buff_pos, const Plan_s & plan)
{
	int ret = SUCCESS;
	Common::QueryResult* query_result = dynamic_cast<Common::QueryResult*>(plan->get_result().get());
	u32 row_size = 0;
	if (query_result) {
		row_size = query_result->size();
	}
	for (u32 i = 0; i < row_size; ++i)
	{
		Row_s row;
		query_result->get_row(i, row);
		RowPacket packet(row);
		packet.set_seq(seq + 1);
		process_single_packet(buff, buff_pos, packet);
	}
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
		Log(LOG_ERR, "RequestHandle", "failed to send error packet to mysql client ret is %d",ret);
	}
	return ret;
}

u32 RequestHandle::do_cmd_query(const String& query)
{
	SqlDriver parser;
	Plan_s plan;

	parser.set_global_database(cur_database);
	int ret = parser.parse_sql(query);
	//Îª±£Ö¤¿Í»§¶Ë¼æÈÝ£¬²»Ö§³ÖµÄSQLÓï·¨·µ»ØOK°ü
	if (parser.is_sys_error()) {
		return send_ok_packet();
		//return send_error_packet(ERR_UNEXPECTED, parser.sys_error());
	}
	else if (parser.is_syntax_error()) {
		//return send_ok_packet();
		return send_error_packet(ERR_UNEXPECTED, parser.syntax_error());
	}
	else if (!parser.parse_result()) {
		return send_ok_packet();
	}
	else{
		plan = Plan::make_plan(parser.parse_result());
		plan->set_thd(m_self);
		u32 ret = plan->optimizer();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return send_error_packet(ERR_UNEXPECTED, result->to_string());
			}
			else {
				return send_error_packet(ERR_UNEXPECTED, "unknown error");
			}
		}
		ret = plan->build_plan();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return send_error_packet(ERR_UNEXPECTED, result->to_string());
			}
			else {
				return send_error_packet(ERR_UNEXPECTED, "unknown error");
			}
		}
		ret = plan->execute();
		if (ret != SUCCESS) {
			Object_s result = plan->get_result();
			if (result) {
				return send_error_packet(ERR_UNEXPECTED, result->to_string());
			}
			else {
				return send_error_packet(ERR_UNEXPECTED, "unknown error");
			}
		}
		else {
			if (plan->send_plan_result()) {
				return send_result_set(plan);
			}
			else {
				return send_ok_packet();
			}
			
		}
	}
}

void RequestHandle::worker_caller(const std::string& func, std::shared_ptr<char> ptr, size_t len)
{
	Log(LOG_TRACE, "RequestHandle", "RequestHandle::worker_caller call func %s( 0x%p, %u)", func.c_str(),ptr.get(), len);
	
	if(!m_write_cache.write_package(ptr.get(),len))
	{
		Log(LOG_ERR, "RequestHandle", "RequestHandle write_cache full,drop %s`s response", func.c_str());
	}
}

void RequestHandle::handle_request(char* buf, size_t len)
{
	enum enum_server_command command;
	command = (enum enum_server_command)(unsigned char)buf[0];
	seq = 0;
	is_com_field_list = false;
	Log(LOG_TRACE, "RequestHandle", "handle client command %u", command);
	switch (command)
	{
		case COM_INIT_DB:
		{
			String db(buf + 1, len - 1);
			String query = "use "+db;
			Task_type task = std::bind(&RequestHandle::do_cmd_query, this, query);
			m_server_service.workers().append_task(task);
			break;
		}
		case COM_QUERY:
		{
			String query(buf + 1, len - 1);
			if (query.find("SHOW SESSION") != String::npos)
					query = "select * from system.sys_vars";
			else if (query.find("SELECT current_user") != String::npos)
					query = "select * from system.current_user";
			else if (query.find("SELECT CONNECTION_ID") != String::npos)
					query = "select * from system.connection_id";
			else if (query.find("show char") != String::npos)
					query = "select * from system.charset";
			else if (query.find("show engines") != String::npos)
					query = "select * from system.engine";
			else if (query.find("show collation") != String::npos)
					query = "select * from system.collation";
			else if (query.find("show variables") != String::npos)
					query = "select * from system.sys_vars";
			else if (query.find("SHOW VARIABLES") != String::npos)
					query = "select * from system.sys_vars";
			else if (query.find("SHOW PROCEDURE") != String::npos)
					{send_ok_packet();return;}
			else if (query.find("SHOW FUNCTION") != String::npos)
					{send_ok_packet();return;}
			else if (query.find("load tpch data") != String::npos)
					{load_tpch_data();return;}
			Task_type task = std::bind(&RequestHandle::do_cmd_query, this, query);
			m_server_service.workers().append_task(task);
			break;
		}
		case COM_PING:
		{
			send_ok_packet();
			break;
		}
		case COM_FIELD_LIST:
		{
			is_com_field_list = true;
			String query = "select * from "+String(buf+1,len-1) + " limit 1";
			Task_type task = std::bind(&RequestHandle::do_cmd_query, this, query);
			m_server_service.workers().append_task(task);
			break;
		}
		default:
			do_not_support();
	}
}

void RequestHandle::load_tpch_data()
{/*
	create_table();
	Task_type task = std::bind(load_lineitem_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_orders_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_partsupp_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_customer_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_part_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_supplier_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_region_data);
	m_server_service.workers().append_task(task);*/
	Task_type task = std::bind(load_nation_data);
	m_server_service.workers().append_task(task);
	task = std::bind(load_partsupp_data);
	m_server_service.workers().append_task(task);
	send_ok_packet();
}