// 修改文件大小
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
    // 打开文件
    int fd = open("trunc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }
    // 像文件中写入数据
    char* buf = "opqrst";
    if(write(fd, buf, strlen(buf)) == -1)
    {
        perror("write");
        return -1;
    }
    // 修改文件大小
    if(truncate("./trunc.txt", 3) == -1)
    {
        perror("truncate");
        return -1;
    }

    // 再次修改文件
    if(ftruncate(fd, 6) == -1)
    {
        perror("ftruncate");
        return -1;
    }
    // 关闭文件
    close(fd);

    return 0;
}