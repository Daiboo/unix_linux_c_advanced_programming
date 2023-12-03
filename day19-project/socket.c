// 通信模块实现
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"



static int s_sock = -1;  // 监听套接字

int initSocket(short port)
{

    printf("%d.%ld > 初始化套接字\n",getpid(),syscall(SYS_gettid));
    s_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(s_sock < 0)
    {
        perror("socket");
        return -1;
    }
    // 端口复用,避免服务器重启无法绑定端口
    printf("%d.%ld > 设置套接字\n",getpid(),syscall(SYS_gettid));
    int on = 1;
    if(setsockopt(s_sock,SOL_SOCKET,SO_REUSEADDR, &on,sizeof(on)) == -1)
    {
        perror("setsockopt");
        return -1;
    }
    printf("%d.%ld > 组织地址结构\n",getpid(),syscall(SYS_gettid));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("%d.%ld > 绑定端口号\n",getpid(),syscall(SYS_gettid));
    if(bind(s_sock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
    {
        perror("bind");
        return -1;
    }
    printf("%d.%ld > 监听套接字\n",getpid(),syscall(SYS_gettid));
    if(listen(s_sock,1024) < 0)
    {
        perror("listen");
        return -1;
    }
    return 0;
}

// 接受客户端的连接请求
int acceptClient(void)
{
    printf("%d.%ld > 等待客户端的连接请求\n",getpid(),syscall(SYS_gettid));
    struct sockaddr_in cli;  // 用来输出客户端的地址结构
    socklen_t len = sizeof(cli);  // 输出地址结构大小
    int conn = accept(s_sock,(struct sockaddr*)&cli,&len);
    if(conn < 0)
    {
        perror("accept");
        return -1;
    }
    printf("%d.%ld > 已经接收到%s.%hu的连接\n",getpid(),syscall(SYS_gettid),inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
    return conn;
}


// 发送响应头
int sendHead(int conn, const char* head)
{
    printf("%d.%ld > 发送响应头\n",getpid(),syscall(SYS_gettid));
    if(send(conn,head,strlen(head),0) < 0)
    {
        perror("send");
        return -1;
    }
    return 0;

}

// 发送响应体
int sendBody(int conn, const char* path)
{
    printf("%d.%ld > 发送响应体\n",getpid(),syscall(SYS_gettid));
    // 打开文件
    int fd = open(path, O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }
    // 读取并发送
    char buf[1024];
    ssize_t len;
    while((len = read(fd,buf,sizeof(buf))) > 0)
    {
       if(send(conn,buf,len,0) < 0)
       {
           perror("send");
           return -1;
       }
    }
    // 循环结束条件2:len=0  len=-1
    if(len == -1)
    {
        perror("len");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;

}

// 接受客户端的http请求
char* recvRequest(int conn)
{
    char* req = NULL;  // 记录存储区的首地址
    ssize_t len = 0;  // 已经接受的总字节数
    while(1)
    {
        char buf[1024] = {};
        ssize_t byte = recv(conn,buf,sizeof(buf),0);
        if(byte == -1)
        {
            perror("recv");
            free(req);
            return NULL;
        }
        if(byte == 0)
        {
            printf("%d.%ld > 客户端断开连接\n",getpid(),syscall(SYS_gettid));
            free(req);
            return NULL;
        }

        // 扩大存储区
        req = realloc(req, len + byte + 1);
        memcpy(req + len,buf,byte+1);
        len += byte;

        // 看所接受的数据中是否有连续的\r\n
        if(strstr(req,"\r\n\r\n") != NULL)
        {
            break;
        }
    }
    return req;
}

// 销毁套接字
void deinitSocket(void)
{
    close(s_sock);
}