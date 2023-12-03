// sleep
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

// 信号处理函数
void sigfun(int signum)
{
    printf("%d号信号处理开始\n",signum);
    sleep(3);
    printf("%d信号处理结束\n",signum);
}

int main(void)
{   
    // 捕获2号信号
    if(signal(SIGINT, sigfun) == SIG_ERR)
    {
        perror("signal");
        return -1;
    }
    printf("%d进程:我要睡觉了\n",getpid());
    int ret = sleep(10);
    printf("%d进程:sleep函数返回%d\n",getpid(),ret);

    return 0;
}