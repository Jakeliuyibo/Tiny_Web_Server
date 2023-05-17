/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:54:04
 * @LastEditors: liuyibo 1299502716@qq.com
 * @LastEditTime: 2023-05-10 16:38:19
 * @FilePath: \Tiny_Web_Server\threadpool\mthreadpool.h
 * @Description: 实现了半同步半异步线程池模型
 */

#ifndef M_THREADPOOL_H
#define M_THREADPOOL_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <atomic>

class mTask
{
public:
    virtual int run() = 0;                                  // 执行任务
};

template <typename T>
class mThreadPool
{
public:
    mThreadPool(int capacityTask, int capacityThreadPool);  // 构造函数
    ~mThreadPool();                                         // 析构函数
    bool addTask(const mTask& task);                        // 添加任务

private:
    static void *worker(void *arg);                         // 工作处理函数
    void run();

private:
    /* 定义：任务队列相关 */
    int                                 n_task_capacity;    // 最大任务数量
    std::queue<mTask*>                  m_taskqueue;        // 任务队列
    std::mutex                          m_mutex;            // 互斥锁：保证多线程向任务队列添加和删除任务的互斥
    std::condition_variable             m_condition;        // 条件变量

    // conditional 保证多个线程获取任务的同步性

    /* 定义：线程相关     */
    int                                 n_thread;           // 线程数量
    std::vector<std::thread>            m_threadgroup;      // 线程组
    std::atomic_bool                    f_running;          
    // 标志位：线程池是否关闭
};


/**
 * @description: 构造函数
 */
template <typename T>
mThreadPool<T>::mThreadPool(int capacityTask, int capacityThreadPool)
    :   n_task_capacity(capacityTask),
        n_thread(capacityThreadPool),
        f_running(false)
{
    if(n_task_capacity<=0 || n_thread<=0)
        throw std::exception();

    /* set running flag    */
    f_running = true;

    /* create thread group */
    for(int iter=0; iter<n_thread; iter++)
    {
        m_threadgroup.emplace_back(std::thread(std::bind(&mThreadPool::worker, this)));
    }
}

/**
 * @description: 析构函数
 */
template <typename T>
mThreadPool<T>::~mThreadPool()
{
    /* 设置作用域为括号内   */
    {
        /* lock task queue       */
        std::unique_lock<std::mutex> lock(m_mutex);

        /* reset running flag    */
        f_running = false;

        /* notify all            */
        m_condition.notify_all();
    }

    /* join thread in threadgroup */
    for(auto &t:m_threadgroup)
    {
        t.join();
    }
    /* release threadgroup        */
    m_threadgroup.clear();
}

/**
 * @description: 添加任务
 */
template <typename T>
bool mThreadPool<T>::addTask(const Task& task)
{
    /* lock task queue       */
    std::unique_lock<std::mutex> lock(m_mutex);

    /* check taskqueue isFull */
    if(m_taskqueue.size() > n_task_capacity)
    {
        return false;
    }

    /* add task to taskqueue */
    m_taskqueue.push(task);    

    /* notify one            */
    m_condition.notify_one();

    return true;
}


/**
 * @description: 工作处理函数
 */
template <typename T>
void* mThreadPool<T>::worker(void *arg)
{
    /* 由于静态成员无法获得private变量，因此通过arg传递参数 */
    mThreadPool *pool = (mThreadPool *)arg;
    pool->run();
    return pool;
}

/**
 * @description: 工作处理函数
 */
template <typename T>
void mThreadPool<T>::run()
{
    while(f_running)
    {
        /* lock task queue       */
        std::unique_lock<std::mutex> lock(m_mutex);

        /*  check whether there is a task in taskqueue,
            if there is a task, execute down.
            otherwise block waiting 
        */
        m_condition.wait(lock);
        if(m_taskqueue.empty())
        {
            std::unique_lock<std::mutex> unlock(m_mutex);
            continue;
        }

        /* get the front task   */
        Task task = m_taskqueue.front();
        m_taskqueue.pop();

        std::unique_lock<std::mutex> unlock(m_mutex);
        if(!task)
        {
            continue;
        }

        /* exec task function   */
        task.process();

    }
}




#endif 
