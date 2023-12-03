// fork+exec
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>


int main(void)
{
    // 创建子进程
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid == 0)
    {
        // 子进程
        if(execl("./new", "new", "hello", "123", NULL) == -1)
        {
            perror("execlp error");
            return -1;
        }
        // return 0;
    }

    // 父进程
    int s;  // 用来输出子进程的终止状态
    if(waitpid(pid, &s, 0) == -1)  // 阻塞
    {
        perror("waitpid error");
        return -1;
    }
    if(WIFSIGNALED(s))
    {
        printf("child process is killed by signal %d\n", WTERMSIG(s));
    }
    else
    {
        printf("child process exit with status %d\n", WEXITSTATUS(s)); 
    }
    
    pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }
    if(pid == 0)
    {
        if(execl("/bin/ls", "ls", "-l", NULL) == -1)
        {
            perror("execl error");
            return -1;
        }
    }
    
    if(waitpid(pid, &s, 0) == -1)
    {
        perror("waitpid error");
        return -1;
    }
    if(WIFEXITED(s))
    {
        printf("child process exit with status %d\n", WEXITSTATUS(s));
    }
    else{
        printf("child process is killed by signal %d\n", WTERMSIG(s));
    }
    
    return 0;
}