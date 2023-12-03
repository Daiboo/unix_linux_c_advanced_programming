// 回收多个子进程
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>  // 分析错误原因

int main(void)
{
    // 创建多个子进程
    for(int i = 0; i < 5; i++)
    {
        pid_t pid = fork();
        if(pid == -1)
        {
            perror("fork");
            return -1;
        }
        if(pid == 0)
        {
            printf("%d进程:我是子进程\n", getpid());
            sleep(i+1);
            return i+1;
        }
    }
    // 回收进程
    while(1)
    {
        int s;   // 用来输出子进程的终止状态
        pid_t pid = wait(&s);
        if(pid == -1)
        {   
            if(errno == ECHILD)
            {
                printf("所有子进程都结束了\n");
                break;
            }
            else
            {
                perror("wait");
                return -1; 
            }
  
        }
        printf("%d进程:回收了%d进程的僵尸\n",getpid(), pid);
        if(WIFEXITED(s))
        {
            printf("正常终止,退出码:%d\n",WEXITSTATUS(s));
        }
        else
        {
            printf("非正常终止,信号:%d\n",WTERMSIG(s));
        }
    }
    return 0;
}