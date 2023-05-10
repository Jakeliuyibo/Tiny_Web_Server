/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:55:02
 * @LastEditors: liuyibo 1299502716@qq.com
 * @LastEditTime: 2023-05-10 16:06:09
 * @FilePath: \Tiny_Web_Server\threadpool\test_for_threadpool.cpp
 * @Description: 测试文件
 */
#include <bits/stdc++.h>
#include "threadpool.h"

using namespace std;


class Task
{
    private:
        int total = 0;
    public:
        Task(){}
        ~Task(){}
        void process()
        {
            std::cout << "task successful！" << std::endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
};


int main()
{
    mThreadPool<Task> pool(8,8);
    for(int ii=0; ii<100; ii++)
    {
        Task task = Task();
        pool.addTask(&task);
        delete task;
    }

    return 0;
}