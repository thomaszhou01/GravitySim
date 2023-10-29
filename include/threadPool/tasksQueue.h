#pragma once
#include <functional>
#include <mutex>
#include <atomic>
#include <queue>
#include <iostream>

//queue for tasks that need to be completed
class TasksQueue {
	std::mutex lock;
	std::atomic<int> tasksInQueue;
	std::queue<std::function<void()>> tasks;

public:
	TasksQueue() = default;

	//add task to queue
	template<typename Lambda>
	void addTask(Lambda&& workerTask) {
		std::lock_guard<std::mutex> lock_guard{ lock };
		tasks.push(workerTask);
		tasksInQueue++;
	}

	void getTask(std::function<void()>& workerTask);
	void doneTask();
	void waitForTasksCompletion();
	static void wait();
};