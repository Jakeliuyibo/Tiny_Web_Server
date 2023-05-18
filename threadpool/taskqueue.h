#ifndef M_TASKQUEUE_H
#define M_TASKQUEUE_H

#include <iostream>
#include <functional>
#include <queue>
#include <mutex>
#include <future>

class mTaskQueue
{
public:
    // 任务类
    using mTask = std::function<void()>;
    // 构造函数
    mTaskQueue();  
    // 析构函数
    ~mTaskQueue();             
    // 提交任务
    template<typename F, typename ...Args>
    auto addTask(F& f, Args&& ...args) -> std::future<decltype(f(args...))>;                            
    // 取出任务
    mTask takeTask();
    // 检测队列是否为空
    inline bool empty()
    {
        /* lock task queue       */
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

private:
    std::queue<mTask>                   m_queue;            // 任务队列
    std::mutex                          m_mutex;            // 互斥锁：保证多线程向任务队列添加和删除任务的互斥
};

/**
 * @description: 构造函数
 */
mTaskQueue::mTaskQueue()
{
    std::cout << "INFO: Task queue 构造函数执行" << std::endl;
}

/**
 * @description: 析构函数
 */
mTaskQueue::~mTaskQueue()
{
    std::cout << "INFO: Task queue 析构函数执行" << std::endl;
}

/**
 * @description: 提交任务
 */
template<typename F, typename ...Args>
auto mTaskQueue::addTask(F& f, Args&& ...args) -> std::future<decltype(f(args...))> 
{
    /* 获取返回值类型        */
    using retType = decltype(f(args...));

    /* 封装任务过程
     *  1、调用bind绑定传入函数和传参，封装为无形参类型
        2、将绑定参数后的函数封装为只有返回值没有形参的任务对象
        3、生成智能指针，离开作用域自动析构
     */
    auto task = std::make_shared<std::packaged_task<retType()>>(std::bind(f, std::forward<Args>(args)...));

    /* lock task queue       */
    std::unique_lock<std::mutex> lock(m_mutex);

    /* 通过lambda表达式，调用封装后的函数，并返回一个无形参无返回值的函数对象
    */
    m_queue.emplace([task]{(*task)();});
    return task->get_future();
} 

/**
 * @description: 取出任务
 */
mTaskQueue::mTask mTaskQueue::takeTask()
{
    /* lock task queue       */
    std::unique_lock<std::mutex> lock(m_mutex);

    if(~m_queue.empty())
    {
        auto task = std::move(m_queue.front());
        m_queue.pop();
        return task;
    }
    else
    {
        std::cout << "ERROR: Task queue is empty" << std::endl;
        return nullptr;
    }
}

#endif 
