// 基于tcp协议的客户端和服务器
#include<stdio.h>
#include<string.h>
#include<ctype.h>  // toupper
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/un.h>

int main(void)
{
    // 创建套接字,买了个手机
    printf("服务器:创建套接字\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("socket error");
        return -1;
    }
    
    // 组织地址结构，办张电话卡
    printf("服务器:组织地址结构\n");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  // 做网络通信
    addr.sin_port = htons(8888);   // 指定端口,字节序转换
    // addr.sin_addr.s_addr = inet_addr("172.27.115.137");   // ip地址,串->整数
    addr.sin_addr.s_addr = INADDR_ANY;   // 接收自己任意ip地址到来的数据
 
    // 绑定套接字和地址结构,手机卡插入手机
    printf("服务器:绑定套接字和地址结构\n");
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("bind error");
        return -1;
    }

    // 开始监听，等待客户端连接
    printf("服务器:开始监听，等待客户端连接\n");
    if(listen(sockfd, 1024) == -1)
    {
        perror("listen error");
        return -1;
    }
    
    // 等待连接,等待客户端的连接请求到来,接受连接请求,完成三次握手
    
    
    printf("服务器:等待客户端连接请求到来\n");
    struct sockaddr_in client_addr;  // 用来输出客户端的地址结构
    socklen_t len = sizeof(client_addr);  // 输出地址结构大小
    int conn = accept(sockfd, (struct sockaddr*)&client_addr, &len);  // accept是阻塞函数
    if(conn == -1)
    { 
        perror("accept error");
        return -1;
    }
    printf("服务器:连接建立成功\n");

    // 业务处理
    printf("服务器:业务处理\n");
    while(1)
    {
        // 接客户端发来的小写的串
        char buf[128] = {};
        ssize_t size = read(conn, buf, sizeof(buf)-1);
        if(size == -1)
        {
            perror("read error");
            return -1;
        }
        if(size == 0)
        {
            // 客户端关闭套接字
            printf("服务器:客户端关闭套接字\n");
            break;
        }
        // 转成大写
        for(int i = 0; i < strlen(buf); i++)
        {
            buf[i] = toupper(buf[i]);
        }
        // 将大写的串发给客户端
        ssize_t ret = write(conn, buf, strlen(buf));
        if(ret == -1)
        {
            perror("write error");
            return -1;
        }
    }

    // 关闭套接字
    printf("服务器:关闭套接字\n");
    close(sockfd);
    close(conn);


    return 0;
}