// 基于udp的客户端
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

int main(void)
{
    // 创建套接字
    printf("客户端:创建套接字\n");
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); // udp的通信
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }

    // 组织服务器地址结构
    printf("客户端:组织服务器地址结构\n");
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9999);
    servaddr.sin_addr.s_addr = inet_addr("172.27.115.137");

    // 业务处理
    printf("客户端:业务处理\n");
    while(1)
    {
        // 通过键盘获取小写的串
        char buf[128] = {};
        fgets(buf, sizeof(buf), stdin);
        if(strcmp(buf, "!\n") == 0)
        {
            break;
        }
        // 发送给服务器
        if(sendto(sockfd, buf, strlen(buf), 0, 
        (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
        {
            perror("sendto");
            return -1;
        }

        // 接受回传的数据
        if(recv(sockfd, buf, sizeof(buf)-1, 0) == -1)
        {
            perror("recv");
            return -1;
        }
        // 显示
        printf("客户端:收到服务器回传的数据: %s", buf);
    }

    // 关闭套接字
    printf("客户端:关闭套接字\n");
    close(sockfd);

    return 0;
}