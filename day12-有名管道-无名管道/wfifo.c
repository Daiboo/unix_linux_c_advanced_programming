// 写入数据
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
    // 创建有名管道文件
    printf("创建有名管道文件\n");
    if(mkfifo("fifo1", 0666) == -1)
    {
        perror("mkfifo");
        return -1;
    }
    

    // 打开有名管道文件
    printf("打开有名管道文件\n");
    int fd = open("fifo1", O_WRONLY);
    if(fd ==  -1)
    {
        perror("open");
        return -1;
    }

    // 循环多次向文件中写入数据
    printf("循环多次向文件中写入数据\n");
    while(1)
    {
        // 通过键盘获取数据
        char buf[64] = {};  // 以空做初始化
        fgets(buf, sizeof(buf), stdin);
        // 人为输入!则退出循环
        if(strcmp(buf, "!\n") == 0)
        {
            break;
        }
        // 写入有名管道
        if(write(fd, buf, strlen(buf)) == -1)
        {
            perror("write");
            return -1;
        }

    }

    // 关闭文件
    printf("关闭文件\n");
    close(fd);

    // 删除有名管道文件
    printf("删除有名管道文件\n");
    unlink("./fifo1");

    return 0;
}