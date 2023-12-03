// 基于udp协议的服务器
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    // 创建套接字
    printf("服务器:创建套接字\n");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // udp
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 组织地址结构
    printf("服务器:组织地址结构\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    // 绑定
    printf("服务器:绑定套接字和地址结构\n");
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("bind");
        return -1;
    }

    // 业务处理
    printf("服务器:业务处理\n");
    while(1)
    {
        // 接受客户端的数据
        char buf[128] = {};
        struct sockaddr_in cliaddr;  // 输出地址结构
        socklen_t len = sizeof(cliaddr);  // 输出大小
        printf("服务器:等待客户端发送数据\n");
        if(recvfrom(sockfd, buf, sizeof(buf)-1, 0,
         (struct sockaddr*)&cliaddr, &len) == -1)
        {
            perror("recvfrom");
            return -1;
        }

        // 转成大写
        for(int i=0; i<strlen(buf); i++)
        {
            buf[i] = toupper(buf[i]);
        }
        // 回传给客户端
        if(sendto(sockfd, buf, strlen(buf), 0,
         (struct sockaddr*)&cliaddr, len) == -1)
        {
            perror("sendto");
            return -1;
        }
    }

    // 关闭套接字
    printf("服务器:关闭套接字\n");
    close(sockfd);

    return 0;
}