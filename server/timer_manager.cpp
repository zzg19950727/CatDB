#include "timer_manager.h"
#include <iostream>
#include <algorithm>

using namespace CatDB::Server;

DateTime::MilliSeconds DateTime::NanoToMilli(const NanoSeconds& nano)
{
	return std::chrono::duration_cast<MilliSeconds, Rep, std::nano>(nano);
}
DateTime::Rep DateTime::now_steady_nano()
{
	TimePoint tp = std::chrono::steady_clock::now();
	NanoSeconds nano(tp.time_since_epoch());
	return nano.count();
}
DateTime::Rep DateTime::now_steady_miil()
{
	TimePoint tp = std::chrono::steady_clock::now();
	NanoSeconds nano(tp.time_since_epoch());
	MilliSeconds milli = NanoToMilli(nano);
	return milli.count();
}
DateTime::Rep DateTime::now_steady()
{
#ifdef NANO
	return now_steady_nano();
#else
	return now_steady_miil();
#endif
}
void DateTime::sleep_nano(unsigned int nano)
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(nano));
}
void DateTime::sleep_milli(unsigned int msec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}
void DateTime::sleep(unsigned int intervals)
{
#ifdef NANO
	sleep_nano(intervals);
#else
	sleep_milli(intervals);
#endif
}

TimerManager::TimerManager()
	:m_actived_timers(0)
{
	m_stop.store(false);
	std::thread tmp(std::bind(&TimerManager::run, this, 100));
	m_thread.swap(tmp);
}
TimerManager::~TimerManager()
{
	m_stop.store(true);
	m_thread.join();
}

void TimerManager::run(unsigned int intervals)
{
	while (!m_stop)
	{
		DateTime::sleep(intervals);
		Rep now = DateTime::now_steady();
		std::vector<Timer*> timers;
		while (m_actived_timers > 0 && m_timers.size())
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			if (m_timers[0].m_expired <= now)
			{
				timers.push_back(m_timers[0].m_timer);

				std::pop_heap(m_timers.begin(),
					m_timers.begin() + m_actived_timers,
					TimerManager::TimerComp);
				--m_actived_timers;

			}
			else
			{
				break;
			}
		}
		for (unsigned int i = 0; i < timers.size(); ++i)
			timers[i]->time_out();
	}
}


void TimerManager::add_timer(Timer* timer)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	for (auto iter = m_timers.begin(); iter != m_timers.end(); ++iter)
	{
		if (iter->m_timer == timer)
		{
			return;
		}
	}
	m_timers.push_back(TimerEntry(timer, 0));
}
void TimerManager::remove_timer(Timer* timer)
{
	disable_timer(timer);
	std::lock_guard<std::mutex> lock(m_mutex);
	for (auto iter = m_timers.begin(); iter != m_timers.end(); ++iter)
	{
		if (iter->m_timer == timer)
		{
			m_timers.erase(iter);
			break;
		}
	}
}
void TimerManager::enable_timer(Timer* timer, Rep expired)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	int pos = -1;
	for (unsigned int i = m_actived_timers; i < m_timers.size(); ++i)
	{
		if (m_timers[i].m_timer == timer)
		{
			pos = i;
			break;
		}
	}
	if (pos < 0)
		return;

	m_timers[pos].m_expired = expired;
	std::swap(m_timers[m_actived_timers], m_timers[pos]);

	++m_actived_timers;
	std::push_heap(m_timers.begin(),
		m_timers.begin() + m_actived_timers,
		TimerManager::TimerComp);
}
void TimerManager::disable_timer(Timer* timer)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	int pos = -1;
	for (int i = 0; i < m_actived_timers; ++i)
	{
		if (m_timers[i].m_timer == timer)
		{
			pos = i;
			break;
		}
	}
	if (pos < 0)
		return;

	std::pop_heap(m_timers.begin() + pos,
		m_timers.begin() + m_actived_timers,
		TimerManager::TimerComp);
	--m_actived_timers;
}
bool TimerManager::TimerComp(const TimerEntry& lhs, const TimerEntry& rhs)
{
	return lhs.m_expired > rhs.m_expired;
}

Timer::Timer(TimerManager& manager, Timer::Type type)
	:m_manager(manager),
	m_interals(-1),
	m_expired(-1),
	m_type(type)
{
	m_manager.add_timer(this);
}

Timer::~Timer()
{
	m_manager.remove_timer(this);
}

void Timer::start(unsigned int intervals)
{
	m_interals = intervals;
	m_expired = DateTime::now_steady() + m_interals;
	m_manager.enable_timer(this, m_expired);
}

void Timer::stop()
{
	m_manager.disable_timer(this);
}

bool Timer::is_time_out()
{
	Rep now = DateTime::now_steady();
	return m_expired <= now;
}

void Timer::time_out()
{
	if(m_func)
		m_func();
	if (m_type == CIRCLE)
	{
		m_expired = DateTime::now_steady() + m_interals;
		m_manager.enable_timer(this, m_expired);
	}
}

template<typename F, typename... ARG>
Timer::CallbackFunc Timer::set_callback_func(F& f, ARG&&... args)
{
	CallbackFunc ret = m_func;
	m_func = std::bind(f, std::forward<ARG>(args)...);
	return ret;
}

Timer::CallbackFunc Timer::set_callback_func(Timer::CallbackFunc& f)
{
	CallbackFunc ret = m_func;
	m_func = f;
	return ret;
}

Timer::CallbackFunc Timer::get_callback_func()
{
	return m_func;
}

Timer::Type Timer::get_type()
{
	return m_type;
}

void Timer::set_type(Timer::Type type)
{
	m_type = type;
}
