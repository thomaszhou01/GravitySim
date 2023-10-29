#include <threadPool/tasksQueue.h>


void TasksQueue::getTask(std::function<void()>& workerTask) {
	std::lock_guard<std::mutex> lock_guard{ lock };
	if (tasks.empty()) {
		return;
	}
	//give worker task
	workerTask = std::move(tasks.front());
	tasks.pop();
}

void TasksQueue::doneTask() {
	tasksInQueue--;
}

void TasksQueue::waitForTasksCompletion(){
	while (tasksInQueue > 0) {
		wait();
	}
}

void TasksQueue::wait() {
	std::this_thread::yield();
}

