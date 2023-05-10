/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:54:04
 * @LastEditors: liuyibo 1299502716@qq.com
 * @LastEditTime: 2023-05-10 10:06:50
 * @FilePath: \Tiny_Web_Server\threadpool\mthreadpool.h
 * @Description: 实现了半同步半异步线程池模型
 */

#ifndef M_THREADPOOL_H
#define M_THREADPOOL_H

#include <thread>

template <typename T>
class mThreadPool
{
    public:
        mThreadPool(int capacityTask, int capacityThreadPool);
        ~mThreadPool();
        void addTask(T &task);

    private:
        static void *worker(void *arg);
        void run();

    private:

        /* 定义：任务队列相关 */
        int                                 n_task_capacity; // 最大任务数量
        std::queue<std::function<void()>>   m_queue;         // 任务队列
        std::mutext                         m_mutex;         // 互斥锁：保证多线程向任务队列添加和删除任务的互斥
        std::condition_variable             m_condition;     // 条件变量

        // conditional 保证多个线程获取任务的同步性

        /* 定义：线程相关     */
        int                                 n_thread;        // 线程数量
        std::vector<std::thread>            m_threadgroup;   // 线程组
        atomic_bool                         f_running;       // 标志位：线程池是否关闭
};


/**
 * @description: 构造函数
 */
template <typename T>
mThreadPool::mThreadPool(int capacityTask, int capacityThreadPool)
    :   n_task_capacity(capacityTask),
        n_thread(capacityThreadPool),
        f_running(true)
{
    if(n_task_capacity<0 || n_thread<0)
        throw std:exception();

}



#endif 
