// 文件的打开和关闭
#include<stdio.h>
#include<fcntl.h> // open
#include<unistd.h> // close


int main()
{
	int fd = open("./open.txt",O_RDWR | O_CREAT |O_TRUNC, 0777);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	printf("fd=%d\n",fd);


	close(fd);
	return 0;
}
