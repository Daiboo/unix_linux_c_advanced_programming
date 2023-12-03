// 子进程的回收
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    // 创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }

    // 子进程代码
    if(pid == 0)
    {
        printf("%d进程:我是子进程,不想死\n", getpid());
        // sleep(5);
        // return 255;
        // exit(300);
        // abort();
        char* p = NULL;
        *p = 10;
    }

    // 父进程代码
    printf("%d进程:等待子进程结束并收尸\n", getpid()); 
    int s;  // 用来输出所回收的子进程的终止状态
    pid_t childpid = wait(&s);  
    if(childpid == -1)
    {
        perror("wait error");
        return -1;
    }
    printf("%d进程:回收了%d进程的僵尸\n",getpid(),childpid);
    if(WIFEXITED(s))
    {
        printf("%d进程:子进程正常终止,退出状态为%d\n",getpid(),WEXITSTATUS(s));
    }
    else
    {
        printf("%d进程:子进程非正常终止,信号:%d\n",getpid(),WTERMSIG(s));
    }


    return 0;
}