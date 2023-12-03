// 从消息队列中读取数据
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <errno.h>


int main(void)
{
    // 合成键
    printf("%d进程:合成键\n",getpid());
    key_t key = ftok(".", 123);
    if(key == -1)
    {
        perror("ftok error");
        return -1;
    }


    // 获取消息队列
    printf("%d进程:获取消息队列\n",getpid());
    int msgid = msgget(key, 0);
    if(msgid == -1)
    {
        perror("msgget error");
        return -1;
    }

    // 接收消息
    printf("%d进程:接收消息\n",getpid());
    while(1)
    {
        struct
        {
            long type;
            char data[64];  // 消息内容
        } buf = {};
        
        ssize_t size = msgrcv(msgid, &buf, sizeof(buf.data)-1, 1234, 0);
        if(size == -1)
        {
            if(errno == EIDRM)
            {
                printf("%d进程:消息队列被删除\n",getpid());
                break;
            }
            else
            {
                perror("msgrcv");
                return -1;                
            }
        }
        printf("%ld>>%s",buf.type,buf.data);
    }   
    printf("%d进程:大功告成\n",getpid());

    return 0;
}