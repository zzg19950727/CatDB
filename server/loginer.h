#ifndef LOGINER_H
#define LOGINER_H
#include <stdint.h>
#include "type.h"

namespace CatDB
{
	namespace Server
	{
		union ClientCapability
		{
			ClientCapability(uint32_t capability);
			uint32_t capability_;
			//ref:http://dev.mysql.com/doc/internals/en/connection-phase.html#packet-Protocol::CapabilityFlags
			struct CapabilityFlags
			{
				uint32_t CLIENT_LONG_PASSWORD : 1;
				uint32_t CLIENT_FOUND_ROWS : 1;
				uint32_t CLIENT_LONG_FLAG : 1;
				uint32_t CLIENT_CONNECT_WITH_DB : 1;
				uint32_t CLIENT_NO_SCHEMA : 1;
				uint32_t CLIENT_COMPRESS : 1;
				uint32_t CLIENT_ODBC : 1;
				uint32_t CLIENT_LOCAL_FILES : 1;
				uint32_t CLIENT_IGNORE_SPACE : 1;
				uint32_t CLIENT_PROTOCOL_41 : 1;
				uint32_t CLIENT_INTERACTIVE : 1;
				uint32_t CLIENT_SSL : 1;
				uint32_t CLIENT_IGNORE_SIGPIPE : 1;
				uint32_t CLIENT_TRANSACTIONS : 1;
				uint32_t CLIENT_RESERVED : 1;
				uint32_t CLIENT_SECURE_CONNECTION : 1;
				uint32_t CLIENT_MULTI_STATEMENTS : 1;
				uint32_t CLIENT_MULTI_RESULTS : 1;
				uint32_t CLIENT_PS_MULTI_RESULTS : 1;
				uint32_t CLIENT_PLUGIN_AUTH : 1;
				uint32_t CLIENT_CONNECT_ATTRS : 1;
				uint32_t CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA : 1;
				uint32_t CLIENT_RESERVED_NOT_USE : 10;
			} CapabilityFlags;
		};

		class Loginer
		{
		public:
			static const int32_t BUFFER_SIZE = 1024 * 1024 * 2; //2MB net buffer
		public:
			struct LoginInfo
			{
				LoginInfo();
				//uint32_t capability_flags_;
				union ClientCapability capability_;
				uint32_t max_packet_size_;
				uint8_t character_set_;
				String user_name_;
				String db_name_;
				String auth_response_;
			};
			Loginer(int client_id, int client_fd);
			~Loginer();
			/**
			* Perform handshake, authorize client
			* @param c   connection to authorize
			*
			*/
			int login();
			const LoginInfo& get_login_info()const;

		private:
			/**
			* send handshake packet to client
			* @param
			*
			*/
			int handshake();

			/**
			* read client auth packet from c->fd
			* do nothing
			* @param c   connection to read data
			*
			*/
			int parse_packet();
			int check_privilege();
			/**
			* write data through raw socket
			* just used to send handshake && ok/error to client
			* @param fd       socket handler
			* @param buffer   data to send
			* @param length   length of data
			*/
			int write_data(int fd, char* buffer, size_t length);
			int read_data(int fd, char* buffer, size_t length);
		private:
			LoginInfo login_info_;
			int client_id;
			int client_fd;
		};
	}
}
#endif		//LOGINER_H
