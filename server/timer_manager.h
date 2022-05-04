#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include <chrono>
#include <thread>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <functional>

namespace CatDB {
	namespace Server {
		struct DateTime
		{
			typedef	long long	Rep;
			typedef std::chrono::duration<Rep, std::milli>		MilliSeconds;
			typedef std::chrono::duration<Rep, std::nano>		NanoSeconds;
			typedef std::chrono::steady_clock::time_point		TimePoint;
			static MilliSeconds NanoToMilli(const NanoSeconds& nano);
			static Rep now_steady_nano();
			static Rep now_steady_miil();
			static Rep now_steady_second();
			static Rep now_steady();
			static Rep steady_second(Rep start);
			static void sleep_nano(unsigned int nano);
			static void sleep_milli(unsigned int msec);
			static void sleep(unsigned int intervals);
		};

		class Timer;
		class TimerManager
		{
		public:
			typedef DateTime::Rep Rep;
			TimerManager();
			~TimerManager();

			void run(unsigned int intervals);

			void add_timer(Timer* timer);
			void remove_timer(Timer* timer);
			void enable_timer(Timer* timer, Rep expired);
			void disable_timer(Timer* timer);
		private:
			TimerManager(const TimerManager&);
			TimerManager& operator=(const TimerManager&);
			struct TimerEntry
			{
				Timer* m_timer;
				Rep m_expired;

				TimerEntry(Timer* timer = 0,
					Rep expired = 0)
					:m_expired(expired),
					m_timer(timer)
				{}
			};
			static bool TimerComp(const TimerEntry& lhs, const TimerEntry& rhs);
			std::vector<TimerEntry> m_timers;
			int m_actived_timers;
			std::thread m_thread;
			std::atomic<bool> m_stop;
			std::mutex m_mutex;
		};

		class Timer
		{
		public:
			typedef DateTime::Rep	Rep;
			typedef std::function<void()>	CallbackFunc;

			enum Type { ONCE = 0, CIRCLE };

			Timer(TimerManager& manager, Type type = CIRCLE);
			~Timer();
			void start(unsigned int intervals);
			void stop();
			bool is_time_out();
			void time_out();

			template<typename F, typename... ARG>
			CallbackFunc set_callback_func(F& f, ARG&&... args);
			CallbackFunc set_callback_func(CallbackFunc& f);
			CallbackFunc get_callback_func();
			Type get_type();
			void set_type(Type type);
		private:
			Timer(const Timer&);
			Timer& operator=(const Timer&);
			TimerManager&	m_manager;
			CallbackFunc	m_func;
			Rep		m_interals;
			Rep		m_expired;
			Type	m_type;
		};
	}
}

#endif	//TIMER_MANAGER_H
