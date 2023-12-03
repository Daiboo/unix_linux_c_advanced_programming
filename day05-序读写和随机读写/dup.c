#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main()
{
    // 打开文件,得到文件描述符 oldfd
    int oldfd = open("./dup.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(oldfd == -1)
    {
        perror("open");
        return -1;
    }
    printf("oldfd = %d\n",oldfd);
    // 赋值文件描述符oldfd,得到新的文件描述符 newfd
    int newfd = dup(oldfd);
    if(newfd == -1)
    {
        perror("dup");
        return -1;
    }
    printf("newfd = %d\n",newfd);

    // 通过oldfd向文件中写入数据hello world!
    char* buf = "hello world!";
    if(write(oldfd, buf, strlen(buf)) == -1)
    {
        perror("write");
        return -1;
    }
    
    // 通过newdf调整文件的读写位置
    if(lseek(newfd, -6, SEEK_END) == -1)
    {
        perror("lseek");
        return -1;
    }
    // 通过oldfd再次向文件写入数据linux!
    buf = "linux!";
    if(write(oldfd, buf, strlen(buf)) == -1)
    {
        perror("write");
        return -1;
    }

    // 关闭文件,两个都需要关闭
    close(oldfd);
    close(newfd);
    return 0;
}