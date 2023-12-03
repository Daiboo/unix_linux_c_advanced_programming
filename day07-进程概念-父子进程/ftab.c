// 子进程复制父进程的文件描述符表
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



int main(void)
{
    // 打开文件
    int fd = open("ftab.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    // 向文件中写入数据:hello world!
    if(write(fd, "hello world!", strlen("hello world!")) == -1)
    {
        perror("write");
        return -1;
    }
    // 父进程创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    
    // 子进程代码调整文件读写位置
    if(pid == 0)
    {
        // 子进程代码调整文件读写位置
        if(lseek(fd, -6, SEEK_CUR) == -1)
        {
            perror("lseek");
            return -1;
        }
        // 关闭,复制了父进程的文件描述符,一定要关闭
        close(fd);
        return 0;
    }
    // 父进程代码再次写入数据linux!
    sleep(1);
    if(write(fd, "linux!", strlen("linux!")) == -1)
    {
        perror("write");
        return -1;
    }
    
    // 父进程关闭文件描述符表
    close(fd);

    return 0;
}