#pragma once
#include <vector>
#include <iostream>
#include <threadPool/tasksQueue.h>
#include <threadPool/worker.h>

//thread pool for multithreaded physics calculations
class ThreadPool {
	int numThreads;
	TasksQueue queue;
	std::vector<Worker> workers;

public:
	ThreadPool(int numThreads);

	//add task to the queue
	template<typename Lambda>
	void addTask(Lambda&& task) {
		queue.addTask(task);
	}

	void waitForTasks();
	~ThreadPool();
};

