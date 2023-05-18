#ifndef M_THREADPOOL_H
#define M_THREADPOOL_H
/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:54:04
 * @LastEditors: liuyibo_ubuntu 1299502716@qq.com
 * @LastEditTime: 2023-05-18 23:00:14
 * @FilePath: \Tiny_Web_Server\threadpool\mthreadpool.h
 * @Description: 实现了半同步半异步线程池模型
 */
#include <vector>
#include <thread>
#include <condition_variable>
#include <exception>
#include <atomic>

#include "taskqueue.h"
// #include "task.h"


class mThreadPool
{
private:
    int                                         num_thread;         // 线程数量
    std::unique_ptr<mTaskQueue>                 p_taskqueue;        // 智能指针：任务队列
    std::vector<std::shared_ptr<std::thread>>   p_threadgroup;      // 智能指针：线程组
    std::mutex                                  mtx_threadpool;     // 互斥锁：线程池
    std::condition_variable                     var_notempty;       // 条件变量：保证多个线程获取任务的同步性
    std::atomic_bool                            f_shutdonw;         // 标志位：线程池是否关闭

public:
    // 构造函数
    mThreadPool(int threadNum);  
    // 析构函数
    ~mThreadPool();             
    // 提交任务
    template<typename F, typename ...Args>
    auto submit(F& f, Args&& ...args) -> decltype(p_taskqueue->addTask(f, std::forward<Args>(args)...));                            
    // 消费者线程执行任务
    void worker();

private:
    // 消费者线程处理任务函数
    void run();

};

/**
 * @description: 构造函数
 */
mThreadPool::mThreadPool(int threadNum)
    :   num_thread(threadNum),
        f_shutdonw(false)
{
    std::cout << "线程池构造， num_thread=" << num_thread << std::endl;

    if(num_thread<=0)
        throw std::exception();

    /* init task queue    */
    p_taskqueue = std::make_unique<mTaskQueue>();

    /* init thread group   */
    for(int iter=0; iter<num_thread; iter++)
    {
        p_threadgroup.push_back(std::make_shared<std::thread>(&mThreadPool::worker, this));
        /* 与主线程脱离 */
        p_threadgroup.back()->detach();
    }
}

/**
 * @description: 析构函数
 */
mThreadPool::~mThreadPool()
{
    std::cout << "线程池析构"  << std::endl;

    /* set shutdonw flag     */
    f_shutdonw = true;
}

/**
 * @description: 提交任务
 */
template<typename F, typename ...Args>
auto mThreadPool::submit(F& f, Args&& ...args) -> decltype(p_taskqueue->addTask(f, std::forward<Args>(args)...))
{
    /* add task and get return      */
    auto ret = p_taskqueue->addTask(f, std::forward<Args>(args)...);

    /* notify worker thread consume */
    var_notempty.notify_one();

    return ret;
}

/**
 * @description: 消费者线程执行任务
 */
void mThreadPool::worker()
{
    std::cout << "线程池创建子线程 , thread_id = " << std::this_thread::get_id() << std::endl;
    while(!f_shutdonw)
    {
        /* lock task queue       */
        std::unique_lock<std::mutex> lock(mtx_threadpool);

        /*  lambda表达式
            如果任务队列为空，则阻塞在此
            如果任务队列非空或者线程池关闭，则向下执行
        */
        var_notempty.wait(lock, [this]
        {
            return !p_taskqueue->empty() || f_shutdonw;
        });

        /* get the task from taskqueue */
        auto task = std::move(p_taskqueue->takeTask());
        lock.unlock();

        /* execute task function    */
        std::cout << "线程池子线程处理任务 , thread_id = " << std::this_thread::get_id() << std::endl;
        task();
    }
    std::cout << "线程池注销子线程 , thread_id = " << std::this_thread::get_id() << std::endl;
}

#endif 
