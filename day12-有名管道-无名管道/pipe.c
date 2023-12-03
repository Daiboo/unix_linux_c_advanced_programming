// 无名管道
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main(void)
{
    // 父进程创建无名管道
    printf("%d进程:父进程创建无名管道\n",getpid());
    int pipefd[2];  // 用来输出无名管道的读端和写端
    if(pipe(pipefd) == -1)
    {
        perror("pipe error");
        return -1;
    }
    printf("pipefd[0]=%d\n",pipefd[0]);
    printf("pipefd[1]=%d\n",pipefd[1]);


    // 父进程创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        return -1;
    }

    // 子进程负责通过无名管道读取数据
    if(pid == 0)
    {   
        printf("%d进程:关闭写端\n",getpid());
        close(pipefd[1]);
        printf("%d进程:接收数据\n",getpid());
        // 子进程负责通过无名管道读取数据
        while(1)
        {
            char buf[64] = {};
            // 通过管道的读端描述符,读取数据
            ssize_t size = read(pipefd[0],buf,sizeof(buf)-1);
            if(size == -1)
            {
                perror("read error");
                return -1;
            }
            else if(size == 0)
            {
                printf("%d进程:写端被关闭\n",getpid());
                break;
            }
            // 显示
            printf("%d进程:接收数据: %s",getpid(),buf);
        }   
        printf("%d进程:关闭读端\n",getpid());
        close(pipefd[0]);
        printf("%d进程:大功告成\n",getpid());
        return 0;
    }

    // 父进程通过无名管道写数据
    printf("%d进程:关闭读端\n",getpid());
    close(pipefd[0]);
    printf("%d进程:发送数据\n",getpid());
    while(1)
    {
        // 从键盘获取数据
        char buf[64] = {};
        fgets(buf,sizeof(buf),stdin);
        if(strcmp(buf,"!\n") == 0)
        {
            break;
        } 
        // 通过写端描述符,向无名管道写入数据
        if(write(pipefd[1],buf,strlen(buf)) == -1)
        {
            perror("write error");
            return -1;
        }
    }
    printf("%d进程:关闭写端\n",getpid());
    close(pipefd[1]);
    // 父进程收尸
    if(wait(NULL) == -1)
    {
        perror("wait error");
        return -1;
    }
    printf("%d进程:已收尸\n",getpid());
    printf("%d进程:大功告成\n",getpid());
    return 0;
}