#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
	ThreadPool(size_t numThreads) :
		stop(false)
	{
		for (size_t i = 0; i < numThreads; ++i)
		{
			workers.emplace_back([this]
				{
					while (true)
					{
						std::function<void()> currTask;
						{
							std::unique_lock<std::mutex> lock(this->taskMutex);
							this->condition.wait(lock, [this] { return this->stop || this->task != nullptr; });
							if (this->stop)
								return;
							currTask = this->task;
						}
						if (currTask)
							currTask();
					}
				});
		}
	}

	void SetTask(std::function<void()>newTask)
	{
		{
			std::unique_lock<std::mutex> lock(taskMutex);
			task = newTask; // Set the shared task
		}
		condition.notify_all(); // Notify all workers
	}
	void ClearTask()
	{
		std::unique_lock<std::mutex> lock(taskMutex);
		task = nullptr; // Clear the shared task
	}

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(taskMutex);
			stop = true;
		}
		condition.notify_all();
		for (std::thread& worker : workers)
			worker.join();
	}

private:
	std::vector<std::thread> workers;
	std::function<void()> task;
	std::mutex taskMutex;
	std::condition_variable condition;
	std::atomic<bool> stop;
};