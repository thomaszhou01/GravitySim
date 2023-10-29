#include <threadPool/worker.h>

Worker::Worker(TasksQueue& queue, int id) : queue{ &queue }, id{ id } {
	workerRunning = true;
	thread = std::thread(&Worker::run, this);
}

void Worker::run() {
	while (workerRunning) {
		queue->getTask(task);
		if (task == nullptr) {
			TasksQueue::wait();
		}
		else {
			//execute task if there is one in queue
			task();
			queue->doneTask();
			task = nullptr;
		}
	}
}

void Worker::stop() {
	workerRunning = false;
	thread.join();
}

