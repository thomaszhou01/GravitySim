#include <threadPool/worker.h>

Worker::Worker(TasksQueue& queue, int id) : queue{ &queue }, id{ id } {
	workerRunning = true;
	thread = std::thread(&Worker::run, this);
}

void Worker::run() {
	while (workerRunning) {
		//std::cout << id << std::endl;
		queue->getTask(task);
		if (task == nullptr) {
			TasksQueue::wait();
		}
		else {
			task();
			queue->doneTask();
			task = nullptr;
		}
	}
}

void Worker::stop() {
	workerRunning = false;
	thread.join();
	std::cout << "thread " << id << " ended" << std::endl;
}