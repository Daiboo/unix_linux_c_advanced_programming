// 太平间信号
#include <stdio.h>
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

typedef void (*sighandler_t)(int);
// 信号处理函数,负责对子进程进行收尸
void sigchld(int signu)
{
    printf("%d进程:捕获到%d号信号\n",getpid(),signu);
    sleep(2);
    while(1)
    {
        pid_t pid = waitpid(-1,NULL,WNOHANG);
        if(pid == -1)
        {
            if(errno == ECHILD)
            {
                printf("%d进程:没有僵尸进程\n",getpid());
                break;
            }
            else
            {
                perror("waitpid error");
                return;
            }
        }
        else if (pid == 0)
        {
            printf("%d进程:没有僵尸进程,干点别的去\n",getpid());
            break;
        }
        else
        {
            printf("%d进程:回收了%d进程的僵尸\n",getpid(),pid);
        }
    }

}

int main(void)
{
    // 对17号信号进行捕获处理
    if(signal(SIGCHLD, sigchld) == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }
    // 创建多个子进程
    for(int i = 0; i < 5; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork error");
            return -1;
        }
        if(pid == 0)
        {
            printf("%d进程:我是子进程\n",getpid());
            sleep(1);
            return 0;
        }
        
    }
    // 父进程忙自己的事情
    while(1);

    return 0;
}