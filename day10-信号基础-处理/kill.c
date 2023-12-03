// kill函数
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<sys/wait.h>

// 信号处理函数signum
void sigfun(int signum)
{   
    printf("%d进程:捕获到%d号信号\n",getpid(), signum);

}

int main(void)
{
    // 创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    // 子进程
    if(pid == 0)
    {
        // 对2号信号进程捕获处理
        // if(signal(SIGINT, sigfun) == SIG_ERR)
        // {   
        //     perror("signal");
        //     return -1;
        // }
        printf("%d进程:我是子进程\n",getpid());
        while(1);
    }

    // 父进程代码,发送2号信号给子进程
    getchar();
    
    if(kill(pid, SIGINT) == -1)
    {
        perror("kill");
        
    }

    getchar();

    if(kill(pid, 0) == -1)
    {
        if(errno == ESRCH)
        {
            printf("子进程不存在\n");
        }
        else
        {
            perror("kill");
            return -1;
        }

    }
    else
    {
        printf("子进程存在\n");  
    }
    // 收尸
    getchar();
    if(wait(NULL) == -1) 
    {
        perror("wait");
        return -1;
    }

    getchar();

    if(kill(pid, 0) == -1)
    {
        if(errno == ESRCH)
        {
            printf("子进程不存在\n");
        }
        else
        {
            perror("kill");
            return -1;
        }

    }
    else
    {
        printf("子进程存在\n");  
    }
    return 0;
}

