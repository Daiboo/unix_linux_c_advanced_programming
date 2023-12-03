#include<stdio.h>
#include<fcntl.h>  // open
#include<unistd.h>  // close write
#include<string.h>

int main()
{
	
	// 打开shared.txt文件
	int fd = open("./shared.txt",O_CREAT | O_APPEND | O_WRONLY,0664);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	// 向文件中写入数据
	char* buf = "llalll";
	ssize_t size = write(fd, buf,strlen(buf));
	if(size == -1)
	{
		perror("write");
		return -1;
	}

	printf("实际向文件中写入%ld个字节\n",size);

	// 关闭文件
	close(fd);

	return 0;

}
