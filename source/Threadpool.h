#pragma once
#include <thread>
#include <memory>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <future>
#include <functional>
#include <stdexcept>

namespace Elite
{
	class Threadpool
	{
	public:
		Threadpool(uint8_t numThreads);

		template<class F, class... Args>
		std::future<typename std::result_of<F(Args...)>::type> PushTask(F&& f, Args&&... args);

		~Threadpool();

	private:
		std::vector<std::thread> m_Workers;

		std::queue<std::function<void()>> m_Tasks;

		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		bool m_Close;
	};

	Threadpool::Threadpool(uint8_t numThreads)
		: m_Close(false)
	{
		for (int i = 0; i < numThreads; ++i)
		{
			m_Workers.emplace_back([this]
			{
				while (!m_Close && m_Tasks.empty())
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> queueLock(m_QueueMutex);
						m_Condition.wait(queueLock, [this] { return m_Close || !m_Tasks.empty(); });
						task = std::move(m_Tasks.front());
						m_Tasks.pop();
					}

					task();
				}
			});
		}
	}

	//Push a task to the queue, this function will accept function objects, functors and lambdas.
	//it will also return a future object if the funtion has a return value.
	template<class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> Threadpool::PushTask(F&& f, Args&&... args)
	{
		using rType = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<rType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		std::future<rType> ret = task->get_future();
		{
			std::unique_lock<std::mutex> queueLock(m_QueueMutex);

			if (m_Close)
				throw std::runtime_error("Trying to push tasks on closing threadpool!");

			m_Tasks.emplace([task]() { (*task)(); });
		}

		m_Condition.notify_one();

		return ret;
	}

	Threadpool::~Threadpool()
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_Close = true;
		}
		m_Condition.notify_all();

		for (std::thread& t : m_Workers)
			t.join();
	}

}