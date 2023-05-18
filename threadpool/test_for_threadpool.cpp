/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:55:02
 * @LastEditors: liuyibo_ubuntu 1299502716@qq.com
 * @LastEditTime: 2023-05-18 21:26:35
 * @FilePath: \Tiny_Web_Server\threadpool\test_for_threadpool.cpp
 * @Description: 测试文件
 */
#include <bits/stdc++.h>
#include "threadpool.h"

using namespace std;


int task_func(int args)
{
    std::cout << "task successful！ args=" << args << std::endl;
    this_thread::sleep_for(chrono::seconds(1));
    return args;
}

int main()
{
    mThreadPool *pool = new mThreadPool(5);
    for(int ii=0; ii<20; ii++)
    {
        auto ret = pool->submit(task_func, ii);
    }
    this_thread::sleep_for(chrono::seconds(10));
    std::cout << "done" << std::endl;

    return 0;
}