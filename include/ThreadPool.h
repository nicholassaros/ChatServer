#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

using namespace std;

class ThreadPool {
private:
    queue<function<void()>> _TaskQueue;
    vector<thread> _Workers;
    mutex mtx;
    condition_variable _conditionVar;
    atomic<bool> _stop;
    int _ThreadCount;

public:
    ThreadPool(int ThreadCount);
    ~ThreadPool();

    // Methods for pushing and poping a function pointer to the task queue
    function<void()> TaskQueuePop();
    void TaskQueuePush(function<void()>); 

    // Function each thred will attemp to exeucte
    void Worker();
};

#endif