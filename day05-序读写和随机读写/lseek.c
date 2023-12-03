#include<stdio.h>
#include<fcntl.h>  // open
#include<string.h>
#include<unistd.h>   // lseek, write, read, close

int main()
{	
	// 打开文件
	int fd = open("./lseek.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	// 写入数据  hello world!
	char* buf = "hello world!";
	if(write(fd, buf, strlen(buf)) == -1)
	{
		perror("write");
		return -1;
	}
	off_t len = lseek(fd,0,SEEK_CUR);
	printf("此时文件的读写位置是%ld\n", len);
	// 调整文件读写位置
	if(lseek(fd, -6, SEEK_END) == -1)
	{

		perror("lseek");
		return -1;
	}
	// 再次写入数据
	buf = "linux!";
	if(write(fd, buf, strlen(buf)) == -1)
	{	
		perror("write");
		return -1;
	}
	// 再次调整文件写入位置
	if(lseek(fd, 8,SEEK_END) == -1)
	{
		perror("lseek");
		return -1;

	}
	// 第三次写入：
	buf = "啦啦啦啦啦";
	if(write(fd, buf, strlen(buf)) == -1)
	{	
		perror("write");
		return -1;
	}
	// 关闭文件
	close(fd);
	return 0;
}
