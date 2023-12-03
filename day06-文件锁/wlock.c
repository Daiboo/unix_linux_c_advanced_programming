// 演示文件之间的写冲突
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{
    // ./a.out hello 
    // 打开文件
    int fd = open("./shared.txt", O_WRONLY | O_CREAT | O_APPEND, 0664);
    if (fd == -1)
    {
        perror("open");
        return -1;
    } 
    // 加锁
    // struct flock lock;
    // lock.l_type = F_WRLCK;
    // lock.l_whence = SEEK_SET;
    // lock.l_start = 0;
    // lock.l_len = 0;  // 一直锁到文件尾部
    // lock.l_pid = -1;
    
    // if(fcntl(fd, F_SETLKW, &lock) == -1)
    // {
    //     perror("fcntl");
    //     return -1;
    // }


    // 写入数据 argv[1] --> "hello",一个一个字符写
    for(int i = 0; i < strlen(argv[1]); i++)
    {
        if(write(fd, &argv[1][i], sizeof(&argv[1][i])) == -1)
        {
            perror("write");
            return -1;
        }
        sleep(1);
    }

    // 解锁
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
