// 信号处理
#include <stdio.h>
#include <signal.h>   // signal()
#include <unistd.h>

typedef void (*sighandler_t)(int);
// 信号处理函数,内核负责对该函数的调用,并传信号的编号给函数
void func(int sig)
{
    printf("%d进程:捕获到%d号信号\n",getpid(),sig);
}

int main(void)
{   
    printf("%d进程:我要开始死循环了\n",getpid());
    // 对2号信号进行忽略处理
    sighandler_t ret = signal(SIGINT, SIG_IGN);
    if(ret == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    // 对2号信号进行捕获处理
    ret = signal(SIGINT, func);
    if(ret == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    printf("ret=%p\n",ret);

    // 对2号信号进行默认处理
    ret = signal(SIGINT, SIG_DFL);
    if(ret == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    printf("ret=%p\n",ret);
    printf("func=%p\n",func);

    while(1);

    return 0;
}

