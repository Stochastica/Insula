#ifndef INSULA_UTIL_PARALLEL_HPP_
#define INSULA_UTIL_PARALLEL_HPP_

#include <atomic>
#include <cassert>
#include <functional>
#include <thread>

#include <iostream>

#include <boost/lockfree/queue.hpp>

namespace ins
{

/**
 * @brief Thread pool for parallel computation
 * @tparam A Type of argument fed into functor f.
 *
 * The type A must be very lightweight (e.g. int or void*). The class is
 * lock-free.
 */
template <typename A>
class ThreadPool final
{
public:
	/**
	 * @param[in] f Work function that will be called in parallel
	 * @param[in] nTasks Max number of tasks in queue
	 * @param[in] nThreads Number of worker threads. If = 0, the
	 *	hardware concurrency is used.
	 */
	ThreadPool(std::function<void (A)> f, int nTasks, int nThreads = 0);
	~ThreadPool();

	void start();
	void halt();

	/**
	 * Loads a new task
	 * @return True if successful
	 */
	bool tasksEmpty() const { return tasks.empty(); }
	bool finishedEmpty() const { return finished.empty(); }
	bool pushTask(A a) { return tasks.push(a); }
	bool popFinished(A& a) { return finished.pop(a); }

private:
	void work(int i);

	std::function<void (A)> f;
	int nThreads;
	std::thread* threads;
	std::atomic_bool* threadDone;
	std::atomic_bool living;

	boost::lockfree::queue<A> tasks;
	boost::lockfree::queue<A> finished;
};

// Implementations

template <typename A>
ThreadPool<A>::ThreadPool(std::function<void (A)> f, int nTasks, int n):
	f(f),
	nThreads(n <= 0 ? std::thread::hardware_concurrency() : n),
	threads(new std::thread[nThreads]), threadDone(new std::atomic_bool[nThreads]),
	living(true),
	tasks(nTasks), finished(nTasks)
{
	for (int i = 0; i < nThreads; ++i)
	{
		threads[i] = std::thread([this,i](){ this->work(i); });
		threadDone[i] = false;
	}
}
template <typename A>
ThreadPool<A>::~ThreadPool()
{
	halt();
	delete[] threads;
}


template <typename A> void
ThreadPool<A>::start()
{
	for (int i = 0; i < nThreads; ++i)
	{
		assert(threads[i].joinable() &&
		       "class ThreadPool: Thread not joinable.");
		threads[i].detach();
	}
}
template <typename A> void
ThreadPool<A>::halt()
{
	living = false;
	for (int i = 0; i < nThreads; ++i) // Wait for threads to finish
	{
		while (!threadDone[i])
			std::this_thread::yield();
	}
}
template <typename A> void
ThreadPool<A>::work(int i)
{
	while (living)
	{
		A arg;
		bool flag = tasks.pop(arg);
		if (!flag)
		{
			std::this_thread::yield();
			continue;
		}
		f(arg);
		finished.push(arg);
	}
	threadDone[i] = true;
}

} // namespace ins

#endif // !INSULA_UTIL_PARALLEL_HPP_
