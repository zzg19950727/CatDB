#ifndef NET_SERVICE_H
#define NET_SERVICE_H
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/select.h>
#endif	//_WIN32
#include <functional>
#include "type.h"

namespace CatDB {
	namespace Server {
		int start_listen(int port, int listen_n);
		int accept_connection(int fd);
		size_t net_read(int fd, char* buf, size_t len);
		size_t net_write(int fd, char* buf, size_t len);
		void net_close(int fd);
		class NetService
		{
		public:
			enum Event { E_READ = 0, E_WRITE, E_RW };
			typedef std::function<void(int, Event)> CallbackFunc;

			NetService(int size);
			~NetService();

			void poll();
			int register_io(int fd, Event e, CallbackFunc& f);
			int unregister_io(int fd, Event e);
			int modify_event(int fd, Event e);

		private:
			void create_poll(int size);
			int add_event(int fd, Event e);
			int remove_event(int fd, Event e);
			void response_event(int fd, Event e);
			typedef std::pair<int, Event> FdEvent;
		private:
			HashMap<FdEvent, CallbackFunc> m_callback_map;
			fd_set read_events, write_events;
		};
	}
}

#endif	//NET_SERVICE_H
