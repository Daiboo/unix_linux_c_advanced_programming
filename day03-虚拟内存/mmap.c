#include<stdio.h>
#include<sys/mman.h>   // mmap  munmap
#include<string.h>
int main()
{
	// 建立映射
	char* start = mmap(NULL,8192,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,0,0);
	if(start == MAP_FAILED){
		perror("mmap");
		return -1;
	}
	// 存数据
	strcpy(start,"扫好后豪豪豪豪后");
	printf("%s\n",start);
	
	// 解除映射
	if(munmap(start, 4096)==-1) 
	{
		perror("munmap");
		return -1;
	}
	// printf("%s\n",start);
	
	char* start2 = start + 4096;
	strcpy(start2, "2222222");
	printf("%s\n",start2);
	if(munmap(start2,4096)==-1)
	{	
		perror("munmap");
		return -1;

	}
	return 0;
}
