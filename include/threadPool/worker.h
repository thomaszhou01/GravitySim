#pragma once
#include <functional>
#include <thread>
#include <threadPool/tasksQueue.h>

//worker class that executes tasks
class Worker {
	int id;
	bool workerRunning;
	std::thread thread;
	std::function<void()> task;
	TasksQueue* queue;
	void run();

public:
	Worker(TasksQueue& queue, int id);
	void stop();
};

