// 信号集
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

// 实现一个字节的打印
void printByte(char byte)
{
    for(int i = 0; i < 8; i++)
    {
        printf("%d",byte & (1 << 7-i) ? 1 : 0);
    }
    printf(" ");
}
// 重复,实现一块存储区所有字节的打印
void printm(void* buf, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {   
        // 倒序输出
        printByte(((char*)buf)[size-1-i]);
        if((i+1) % 8 == 0)
        {
            printf("\n");
        }
    }
}

int main(void)
{
    sigset_t s;
    // 填满信号机
    printf("填满信号集\n");
    if(sigfillset(&s) == -1)
    {
        perror("sigfillset");
        return -1;
    }
    printm(&s, sizeof(s));

    printf("清空信号集\n");
    sigemptyset(&s);
    printm(&s, sizeof(s));

    printf("添加2号信\n");
    sigaddset(&s, SIGINT);
    printm(&s, sizeof(s));

    printf("添加三号信号\n");
    sigaddset(&s, SIGQUIT);
    printm(&s, sizeof(s));

    printf("删除2号信号\n");
    sigdelset(&s, SIGINT);
    printm(&s, sizeof(s));

    printf("判断信号\n");
    printf("信号集中%s2号信号\n",sigismember(&s, SIGINT) ? "有" : "无");
    printf("信号集中%s3号信号\n",sigismember(&s, SIGQUIT) ? "有" : "无");
    



    return 0;
}