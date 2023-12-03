// 向消息队列写入数据
#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

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
    
    // 创建消息队列
    printf("%d进程:创建消息队列\n",getpid());
    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0664);
    if(msgid == -1)
    {
        perror("msgget error");
        return -1;
    }

    // 发送数据
    printf("%d进程:发送数据\n",getpid());
    while(1)
    {
        struct {
            long type;  // 消息类型
            char data[64];  // 数据内容
        } buf = {1234, ""};  // 空串
        // 通过键盘获取数据
        fgets(buf.data, sizeof(buf.data), stdin);

        if(strcmp(buf.data, "!\n") == 0)
        {
            break;
        }
        // 发送消息
        if(msgsnd(msgid, &buf, strlen(buf.data), 0) == -1)
        {
            perror("msgsnd error");
            return -1;
        }
    }
    // 销毁消息队列
    printf("%d进程:销毁消息队列\n",getpid());
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        perror("msgctl error");
        return -1;
    }

    return 0;
}