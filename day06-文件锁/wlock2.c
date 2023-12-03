// 演示文件之间的写冲突
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    // ./a.out hello 
    // 打开文件
    int fd = open("./shared.txt", O_WRONLY | O_CREAT, 0664);
    if (fd == -1)
    {
        perror("open");
        return -1;
    } 
    // 非阻塞加锁
    // struct flock lock;
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;
    // lock.l_pid = -1;
    
    // while(fcntl(fd, F_SETLK, &lock) == -1)
    // {
    //     if(errno == EACCES || errno == EAGAIN)
    //     {
    //         printf("文件被锁定\n");
    //     }
    //     else
    //     {
    //         perror("fcntl");
    //         return -1;
    //     }
    // }

    // 写入数据 argv[1] --> "hello",一个一个字符写
    // char *buf = "hello world"; 
    if(write(fd,"hello", strlen("hello") == -1))
    {
        perror("write");
        return -1;
    }


    // // 解锁
    // lock.l_type = F_UNLCK;
    // if(fcntl(fd, F_SETLKW, &lock) == -1)
    // {
    //     perror("fcntl");
    //     return -1;
    // }

    
    //关闭文件
    close(fd);

    return 0;



}
