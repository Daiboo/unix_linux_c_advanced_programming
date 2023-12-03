// http模块头文件
#ifndef __HTTP_H
#define __HTTP_H
#include<limits.h>
#include<sys/types.h>
// http请求中的关键数据
typedef struct httpRequest
{
    char method[32];  // 请求方法
    char path[PATH_MAX + 1];  // 资源路径
    char protocol[32];   // 协议版本
    char connection[32];  // 连接状态
} HTTP_REQUEST;


// 解析http请求
int parseRequest(const char* request, HTTP_REQUEST* hreq);

// 构造响应头的关键数据
typedef struct httpResponse
{
    char protocol[32];  // 协议版本
    int status;  // 响应状态
    char desc[64];  //响应码
    char type[32];  // 内容类型
    off_t  length;  // 长度
    char connection[32];  // 连接状态
}HTTP_RESPONSE;
// 构造http响应
int constructHeader(const HTTP_RESPONSE* hres, char* header);


#endif  // __HTTP_H