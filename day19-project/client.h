// 线程模块头文件
#ifndef THREAD_H
#define THREAD_H

// 通信套接字和资源所在路径,线程需要的参数
typedef struct clientArgs
{
    const char* home;  // 资源在本地的存储路径
    int conn;  // 通信套接字
}CA;


// 线程过程函数负责和客户端通信
void* client(void* arg);
#endif 