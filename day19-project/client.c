// 线程模块实现
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"
#include "socket.h"
#include "resource.h"
#include "client.h"


// 线程过程函数
void* client(void* arg)
{
    CA* ca = (CA*)arg;
    printf("%d.%ld > 客户机线程处理开始\n",getpid(),syscall(SYS_gettid));
    while(1)
    {
        printf("%d.%ld > 接收请求\n",getpid(),syscall(SYS_gettid));
        char* req = recvRequest(ca->conn);
        if(req == NULL)
            break;
        printf("%d.%ld > 请求电文:\n%s\n",getpid(),syscall(SYS_gettid),req);

        printf("%d.%ld > 解析请求\n",getpid(),syscall(SYS_gettid));
        HTTP_REQUEST hreq;  // 输出解析的关键数据
        if(parseRequest(req,&hreq) == -1)
        {
            free(req);  // req是分配的存储区
            break;
        }
        free(req);  // 后续操作hreq即可

        // 处理路径
        // 资源路径
        char root[PATH_MAX + 1];
        // 真实路径
        char path[PATH_MAX + 1];
        strcpy(root,ca->home);
        if(root[strlen(root)-1] == '/')
        {
            root[strlen(root)-1] = '\0';
        }
        strcpy(path, root);
        strcat(path,hreq.path);
        // 如果请求路径是/,则应将首页文件响应给对方
        if(strcmp(hreq.path,"/") == 0)
        {   
            strcat(path, "index.html");
        }

        // 构造响应所需的结构
        HTTP_RESPONSE hres = {
            "HTTP/1.1",200,"OK","text/html"
        };
        // 搜索资源
        if(searchResource(path) == -1)
        {
            hres.status = 404;
            strcpy(hres.desc,"Not Found");
            strcpy(path,root);
            strcat(path,"/404.html");
        }
        // 判断类型
        if(identifyType(path,hres.type) == -1)
        {
            hres.status = 404;
            strcpy(hres.desc,"Not Found");
            strcpy(path,root);
            strcat(path,"/404.html");
        }
        // 文件长度
        struct stat st;  // 用来输出文件的元数据
        if(stat(path, &st) == -1)
            break;
        hres.length = st.st_size;
        // 连接状态
        if(strlen(hreq.connection) > 0)
        {
            strcpy(hres.connection,hreq.connection);
        }else if(strcmp(hreq.protocol,"HTTP/1.0") == 0)
        {   
            strcpy(hres.connection,"close");
        }else
        {
            strcpy(hres.connection,"keep-alive");
        }
        // 构造响应头
        printf("%d.%ld > 构造响应头\n",getpid(),syscall(SYS_gettid));
        char header[1024];  // 存储构造好的响应头
        if(constructHeader(&hres,header) == -1)
        {
            break;
        }
        printf("%d.%ld > 响应电文:\n%s\n",getpid(),syscall(SYS_gettid),header);
        // 发送响应
        printf("%d.%ld > 发送响应\n",getpid(),syscall(SYS_gettid));
        if(sendHead(ca->conn,header) == -1)
        {
            break;
        }
        printf("%d.%ld > 发送响应体\n",getpid(),syscall(SYS_gettid));
        if(sendBody(ca->conn,path) == -1)
        {
            break;
        }
        // 如果连接是close
        if(strcasecmp(hres.connection, "close") == 0)
        {
            break;
        }
    }
    close(ca->conn);
    free(ca);
    printf("%d.%ld > 客户机线程处理结束\n",getpid(),syscall(SYS_gettid));
    return NULL;

}

