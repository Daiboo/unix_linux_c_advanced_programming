// http模块实现
#include "http.h"
#include <sys/syscall.h>
#include <stdio.h>
#define __USE_GNU
#include <string.h>  // 为了使用case版本的函数
#include <time.h>
#include <unistd.h>


// 解析请求
int parseRequest(const char* request, HTTP_REQUEST* hreq)
{
    sscanf(request, "%s %s %s", hreq->method, hreq->path, hreq->protocol);
    char* con = strcasestr(request, "connection");  // 忽略大小写敏感的
    if(con)
    {
        sscanf(con, "%*s%s", hreq->connection);
    }
    printf("%d.%ld > [%s][%s][%s][%s]\n",getpid(),syscall(SYS_gettid),
    hreq->method,hreq->path,hreq->protocol,hreq->connection
    );
    // 判断是否为get方法
    if(strcasecmp(hreq->method, "GET") != 0)
    {
        printf("%d.%ld > 无效的方法\n",getpid(),syscall(SYS_gettid));
        return -1;
    }
    // 判断协议版本
    if(strcasecmp(hreq->protocol, "HTTP/1.1") && strcasecmp(hreq->protocol, "HTTP/1.0"))
    {
        printf("%d.%ld > 无效的协议版本\n",getpid(),syscall(SYS_gettid));
        return -1;
    }
    return 0;
}

// 构造响应头
int constructHeader(const HTTP_RESPONSE* hres, char* header)
{
    char dateTime[32];
    time_t now = time(NULL);
    strftime(dateTime, sizeof(dateTime), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
    sprintf(header, "%s %d %s\r\n"
                    "Server: daibooshuai\r\n"
                    "Date: %s\r\n"
                    "Content-Type: %s\r\n"
                    "Content-Length: %ld\r\n"
                    "Connection: %s\r\n"
                    "\r\n", hres->protocol, hres->status, hres->desc, dateTime, hres->type, hres->length, hres->connection);
    return 0;
}

