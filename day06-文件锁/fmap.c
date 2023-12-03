// 内存映射文件
#include <stdio.h>
#include <sys/mman.h>  // 映射文件
#include <string.h>  
#include <fcntl.h>  // 打开文件
#include <unistd.h>  // 关闭文件


int main(void)
{

    // 打开文件
    int fd = open("./fmap.txt", O_RDWR | O_CREAT | O_TRUNC, 0664);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }
    // 修改文件大小(注意映射是有空间的)
    if (ftruncate(fd, 4096) == -1)
    {
        perror("ftruncate");
        return -1;
    }
    // 建立和文件的映射
    char* p = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (p == MAP_FAILED)
    {   
        perror("mmap");
        return -1;
    }
    
    // 修改映射后的内存
    strcpy(p, "hello world 你好世界");
    printf("%s\n",p);
    // 解除映射
    if(munmap(p, 4096) == -1)
    {
        perror("munmap");
        return -1;
    }

    // 关闭文件
    close(fd);

    return 0;
}