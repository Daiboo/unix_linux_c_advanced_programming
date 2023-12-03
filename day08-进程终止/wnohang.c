// 以非阻塞方式收尸
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    // 创建子进程,暂时不结束
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }
    // 子进程代码
    if(pid == 0)
    {
        printf("%d进程:我是子进程,你收不了我\n", getpid());
        sleep(10);
        return 0;
    }

    // 父进程代码
    printf("%d进程:等待子进程结束并收尸\n", getpid());
    while(1)
    {
        pid_t childpid = waitpid(pid,NULL,WNOHANG);
        if(childpid == -1)
        {
            perror("waitpid error");
            return -1;
        }
        else if(childpid == 0)
        {
            printf("%d进程:子进程在运行,收不了,干点别的去\n", getpid());
            sleep(1);
        }
        else
        {
            printf("%d进程:回收了%d进程的僵尸\n", getpid(),childpid); 
            break;
        }
    }
    

    return 0;
}