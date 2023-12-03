// 读取有名管道
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    // 打开有名管道文件
    printf("打开有名管道文件\n");
    int fd = open("./fifo1",O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }
    


    // 读取有名管道文件
    printf("读取有名管道文件\n");
    while(1)
    {
        char buf[64] = {};
        // 从管道中读取数据
        ssize_t size = read(fd,buf,sizeof(buf)-1);
        if(size == -1)
        {
            perror("read");
            return -1;
        }
        else if(size == 0)
        {
            printf("对方关闭管道文件\n");
            break;
        }
        // 输出
        printf("%s",buf);
    }
    // 关闭有名管道文件
    printf("关闭有名管道文件\n");
    close(fd);



    return 0;
}