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
		std::future<typename std::result_of<F(Args...)>::type> Enqueue(F&& f, Args&&... args);

		void WaitFinish();

		~Threadpool();
		
	private:
		std::vector<std::thread> m_Workers;

		std::atomic<int> m_BusyWorkers;
		
		std::queue<std::function<void()>> m_Tasks;

		std::mutex m_QueueMutex;
		std::condition_variable m_cvQueue;
		std::condition_variable m_cvWait;
		bool m_Close;
	};

	Threadpool::Threadpool(uint8_t numThreads)
		: m_Close(false)
		, m_BusyWorkers()
	{
		for (int i = 0; i < numThreads; ++i)
		{
			m_Workers.emplace_back([this]
			{
				for(;;)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> queueLock(this->m_QueueMutex);
						this->m_cvQueue.wait(queueLock, [this] { return this->m_Close || !this->m_Tasks.empty(); });
						if (this->m_Close && this->m_Tasks.empty())
							return;
						++this->m_BusyWorkers;
						task = std::move(this->m_Tasks.front());
						this->m_Tasks.pop();
					}


					task();
					{
						std::unique_lock<std::mutex> queueLock(this->m_QueueMutex);
						--m_BusyWorkers;
						this->m_cvWait.notify_one();
					}
				}
			});
		}
	}

	//Push a task to the queue, this function will accept function objects, functors and lambdas.
	//it will also return a future object if the function has a return value.
	template<class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> Threadpool::Enqueue(F&& f, Args&&... args)
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

		m_cvQueue.notify_one();

		return ret;
	}

	inline void Threadpool::WaitFinish()
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_cvWait.wait(lock, [this]() { return (this->m_BusyWorkers == 0) && this->m_Tasks.empty(); });
	}

	Threadpool::~Threadpool()
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_Close = true;
		}
		m_cvQueue.notify_all();

		for (std::thread& t : m_Workers)
			t.join();
	}

}