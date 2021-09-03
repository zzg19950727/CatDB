#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <string>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include "log.h"
template<typename Callback, typename F, typename... ARG>
struct is_correct_callback
{
private:
	static auto check(int)->decltype(std::declval<Callback>()(std::declval<F>()(std::declval<ARG>()...)), std::true_type());
public:
	static const bool value = std::is_same<decltype(check(0)), std::true_type>::value;
};
typedef std::function<void(void)>	Task_type;
class TaskQueue
{
public:

	TaskQueue()
	{

	}
	~TaskQueue()
	{

	}

	template<typename Callback, typename F, typename... ARG>
	typename std::enable_if<is_correct_callback<Callback, F, ARG&&...>::value, void>::type
		append_task(Callback& callback, F& f, ARG&&... args)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		auto f1 = std::bind(f, std::forward<ARG>(args)...);
		m_tasks.push([=]() {callback(f1()); });
	}

	void append_task(Task_type& task)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_tasks.push(task);
	}

	Task_type take_task()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_tasks.empty())
		{
			return Task_type();
		}
		else
		{
			Task_type task = m_tasks.front();
			m_tasks.pop();
			return task;
		}
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		while (!m_tasks.empty())
			m_tasks.pop();
	}

private:
	std::queue<Task_type> m_tasks;
	std::mutex m_mutex;
};

class Condition
{
public:
	Condition()
		:m_ready(-1)
	{}

	void wait(int target = 1)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&]()->bool {return this->m_ready == target; });
		m_ready = -1;
	}
	void notify_one(int target = 1)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_ready = target;
		}
		m_condition.notify_one();
	}
	void notify_all(int target = 1)
	{
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_ready = target;
		}
		m_condition.notify_all();
	}
private:
	std::condition_variable m_condition;
	std::mutex m_mutex;
	int m_ready;
};

class ThreadPool
{
public:
	ThreadPool(int threads)
		:m_workers(threads)
	{
		m_exit.store(false);
		m_running.store(0);
		m_stop_count.store(0);

		for (int i = 0; i < threads; ++i)
		{
			std::thread tmp(std::bind(&ThreadPool::do_work, std::ref(*this)));
			m_workers[i].swap(tmp);
		}
	}
	~ThreadPool()
	{
		
	}

	template<typename Callback, typename F, typename... ARG>
	typename std::enable_if<is_correct_callback<Callback, F, ARG&&...>::value, void>::type
		append_task(Callback& callback, F& f, ARG&&... args)
	{
		if (!m_exit.load())
		{
			m_tasks.append_task(callback, f, std::forward<ARG>(args)...);
			m_condition.notify_one();
		}
	}

	void append_task(Task_type& task)
	{
		if (!m_exit.load())
		{
			m_tasks.append_task(task);
			m_condition.notify_one();
		}
	}

	void quit()
	{
		m_tasks.clear();
		m_exit.store(true);
		LOG_ERR("wait for all workers exit...", K(m_workers.size()));
		while (m_stop_count.load() != m_workers.size()) {
			m_condition.notify_all();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		LOG_ERR("all workers exit");
	}

private:
	void do_work()
	{
		for (;;)
		{
			Task_type task = m_tasks.take_task();
			if (!task && !m_exit.load())
			{
				m_condition.wait();
				continue;
			}
			else if (!task && m_exit.load())
			{
				break;
			}
			else
			{
				++m_running;
				task();
				--m_running;
			}
		}
		++m_stop_count;
	}
	std::vector<std::thread> m_workers;
	Condition m_condition;
	TaskQueue m_tasks;
	std::atomic_bool m_exit;
	std::atomic<int> m_running;
	std::atomic<int> m_stop_count;
};

#endif	//THREAD_POOL_H
