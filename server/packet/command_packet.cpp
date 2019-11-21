#include "command_packet.h"
#include "log.h"
using namespace CatDB::Server;

CommandPacket::CommandPacket() 
	: type_(0), req_(NULL), next_(NULL), receive_ts_(-1)
{

}

CommandPacket::~CommandPacket()
{

}

int32_t CommandPacket::get_command_length() const
{
	return command_.length();
}

int CommandPacket::set_request(Request_s req)
{
	int ret = SUCCESS;
	if (!req)
	{
		Log(LOG_ERR, "CommandPacket", "invalid argument reqeust is %p", req);
		ret = INVALID_ARGUMENT;
	}
	else
	{
		req_ = req;
	}
	return ret;
}

void CommandPacket::set_command(char* data, const int32_t length)
{
	command_ = String(data, length);
}
void CommandPacket::set_receive_ts(const int64_t &now)
{
	receive_ts_ = now;
}
int64_t CommandPacket::get_receive_ts() const
{
	return receive_ts_;
}