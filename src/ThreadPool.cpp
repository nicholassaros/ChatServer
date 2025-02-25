#include "../include/ThreadPool.h"

#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <atomic>

using namespace std;


ThreadPool::ThreadPool(int ThreadCount)
    : _ThreadCount(ThreadCount), _stop(false) {
    // fill the workers vector with threads which have an action pointing to Worker()
    for (int i = 0; i < _ThreadCount; i++) {
        // creates threads and links them to the Worker method
        _Workers.push_back(thread(&ThreadPool::Worker, this));
    }
}

void ThreadPool::TaskQueuePush(function<void()> task) {
   
    // to push to the queue we need to get a lock
    unique_lock<mutex> lock(mtx);
    _TaskQueue.push(task);
    _conditionVar.notify_one();  // Notify one waiting worker
}

function<void()> ThreadPool::TaskQueuePop() {
    unique_lock<mutex> lock(mtx);

    // We need to check if the queue is empty and if so the thread should wait
    while (_TaskQueue.empty() && !_stop) {
        _conditionVar.wait(lock); // Release lock and wait for notification
    }

    // if stop if active we will break from the while loop
    if(_stop){
        // if the stop command is active we return out
        return nullptr;
    }


    function<void()> task = _TaskQueue.front();
    _TaskQueue.pop();
    return task;
}

void ThreadPool::Worker() {
    while (!_stop) {
        // Check if the stop flag is set before trying to pop from the queue

        function<void()> worker_task = TaskQueuePop();

        if (_stop || !worker_task) {  // If the pool is stopping and no task
            return;  // Exit worker thread
        }

        worker_task();  // Execute the task
    }
}

ThreadPool::~ThreadPool() {

    _stop = true;  // Set the stop flag


    _conditionVar.notify_all();  // Notify all threads to wake up

    for (auto& worker : _Workers) {
        worker.join();  // Join all worker threads to ensure they finish
    }
}