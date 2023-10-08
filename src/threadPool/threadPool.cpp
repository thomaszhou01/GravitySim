#include <threadPool/threadPool.h>

ThreadPool::ThreadPool(int numThreads) : numThreads{ numThreads } {
	workers.reserve(numThreads);
	for (int i = 0; i < numThreads; i++) {
		workers.emplace_back(queue, i);
	}
}

void ThreadPool::waitForTasks() {
	queue.waitForTasksCompletion();
}

ThreadPool::~ThreadPool() {
	for (auto& worker : workers) {
		worker.stop();
	}
}