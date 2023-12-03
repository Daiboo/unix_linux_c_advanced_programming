// 创建子进程
#include <stdio.h>
#include <unistd.h>


int main(void)
{
    printf("%d进程:我是父进程,我要创建子进程\n",getpid());
    // 子进程一旦创建成功,fork之后的代码,父子进程都会执行一遍
    pid_t a = fork();  // 创建子进程
    if(a == -1)
    {
        perror("fork error");
        return -1;
    }
    // 子进程代码
    if(a == 0)
    {
        printf("%d进程:我是子进程,我要喝肥宅 快乐水\n",getpid());
        return 0;
    }

    // 父进程代码
    printf("%d进程:我是父进程,我要喝肥宅快乐水\n",getpid());
    return 0;
    

    // if(a == 0)
    // {
    //     printf("%d进程:我是子进程,我要喝肥宅快乐水\n",getpid());
    //     return 0;
    // }
    // else
    // {
    //     printf("%d进程:我是父进程,我要喝肥宅快乐水\n",getpid());
    //     return 0;
    // }
    // printf("%d进程:肥宅快乐水\n",getpid());  // 被打印了两遍,父进程打了一遍,子进程
    
    // return 0;
}