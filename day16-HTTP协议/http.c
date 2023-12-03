// 完成客户端,向百度服务器发送http请求
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>


int main(int argc, char *argv[])
{
    // ./a.out <ip地址> <域名> [<资源路径>]
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <ip> <domain> [<resource>]\n", argv[0]);
        return -1;
    }
    char* ip = argv[1];
    char* name = argv[2];
    char* path = (argc > 3) ? argv[3] : "";
    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 组织服务器的地址结构
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr(ip);

    // 发起连接
    int conn = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(conn < 0)
    {
        perror("connect");
        return -1;
    }

    // 组织请求
    char request[1024] = {};
    sprintf(request, "GET /%s HTTP/1.1\r\n "
                    "Host: %s\r\n"
                    "Accept: */*\r\n"
                    "Connection: close\r\n"
                    "User-Agent: Mozilla/5.0\r\n\r\n",path, name);
    
    // 发送请求
    if(send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("send");
        return -1;
    }

    // 接受响应
    while(1)
    {
        char respond[1024] = {};
        ssize_t size = recv(sockfd, respond, sizeof(respond)-1, 0);
        if(size == -1)
        {
            perror("recv");
            return -1;
        }
        if(size == 0)
        {
            break;
        }
        printf("%s",respond);
    }
    printf("\n");

    // 关闭套接字
    close(sockfd);


    return 0;
}