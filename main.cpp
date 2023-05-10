/*
 * @Author: liuyibo 1299502716@qq.com
 * @Date: 2023-05-09 17:41:29
 * @LastEditors: liuyibo 1299502716@qq.com
 * @LastEditTime: 2023-05-10 17:04:33
 * @FilePath: \Tiny_Web_Server\main.cpp
 * @Description: Web Server main function
 */
#include <bits/stdc++.h>
#include <winsock2.h>
#include "config.h"
#include "./threadpool/threadpool.h"
#include "./http/http_conn.h"


#define MAX_FILE_DESCRIPTION 100    // 支持的客户端连接数

int main()
{
    // /* 1、创建数据库连接   */
    // // a. 建立连接
    // // b. 数据库连接池
    
    /* 2、创建web服务器    */
    mThreadPool<mHttpConn> *pool = NULL;
    try
    {
        pool = new mThreadPool<mHttpConn>(8,8);
    }
    catch(...)
    {
        exit(-1);
    }

    mHttpConn* users = new mHttpConn[MAX_FILE_DESCRIPTION];
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);


    // a. 初始化web服务器
    // b. 初始化日志模块
    /* 3、开启监听         */

    std::cout << 23435 << std::endl;

    return 0;
}