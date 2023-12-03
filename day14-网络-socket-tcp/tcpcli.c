// 基于tcp的客户端
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(void)
{
    // 创建套接字
    printf("客户端:创建套接字\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 组织地址服务器的地址结构
    printf("客户端:组织服务器地址结构\n");
    struct sockaddr_in addr_server;
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(8888); 
    addr_server.sin_addr.s_addr = inet_addr("172.27.115.137");   // 服务器ip地址

    // 发起连接请求
    printf("客户端:发起连接请求\n");
    int ret = connect(sockfd, (struct sockaddr *)&addr_server, 
    sizeof(addr_server));
    if(ret == -1)
    {
        perror("connect");
        return -1;
    }

    // 业务处理
    printf("客户端:业务处理\n");
    while(1)
    {
        // 获取小写的串
        char buf[128] = {};
        fgets(buf, sizeof(buf), stdin);
        if(strcmp(buf, "!\n") == 0)
        {
            break;
        }
        // 发送给服务器
        if(send(sockfd, buf, strlen(buf), 0) == -1)
        {
            perror("send");
            return -1;
        }
        // 接受大写的串
        if(recv(sockfd, buf, sizeof(buf)-1, 0) == -1)
        {
            perror("recv");
            return -1;
        }
        // 显示输出
        printf("客户端:接受大写的串:%s", buf);
    }

    // 关闭套接字
    printf("客户端:关闭套接字\n");
    close(sockfd);



    return 0;
}


