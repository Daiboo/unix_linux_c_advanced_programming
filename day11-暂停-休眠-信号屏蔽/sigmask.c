// 信号屏蔽
#include<stdio.h>
#include<signal.h>
#include<unistd.h>

// 假装更新数据库
void update_db(void)
{
    for(int i = 0; i < 5; i++)
    {
        printf("正在更新第%d条数据......\n",i+1);
        sleep(1);
    }
}

// 信号处理函数
void sigfun(int signum)
{
    printf("捕获到%d号信号\n",signum);
}

int main(void)
{   
    // int signum = SIGINT;
    int signum = 40;
    // 父进程对2号信号进行捕获处理
    printf("%d进程:捕获%d信号\n",getpid(), signum);
    if(signal(signum,sigfun) == SIG_ERR)
    {
        perror("signal error");
        return -1;
    }

    // 父进程屏蔽2号信号
    printf("%d进程:屏蔽%d号信号\n",getpid(), signum);
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,signum);
    sigset_t oldset;
    if(sigprocmask(SIG_SETMASK,&sigset,&oldset) == -1)  // 设置2号信号屏蔽
    {
        perror("sigprocmask error");
        return -1;
    }
    // 父进程创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }

    // 子进程负责向父进程多次发送2号信号
    if(pid == 0)
    {
        for(int i = 0; i < 5; i++)
        {
            printf("%d进程:给父进程发送%d号信号\n",getpid(),signum);
            if(kill(getppid(),signum) == -1)
            {
                perror("kill error");
                return -1;
            }
        }
        return 0;
    }


    // 父进程开始更新数据库
    update_db();

    // 父进程解除对2号信号的屏蔽
    printf("%d进程:解除对%d号信号的屏蔽\n",getpid(), signum);
    if(sigprocmask(SIG_SETMASK,&oldset,NULL) == -1) 
    {
        perror("sigprocmask error");
        return -1;
    }
 
    // 解除屏蔽之后,不结束,要执行信号处理函数的
    while(1);

    return 0;
}