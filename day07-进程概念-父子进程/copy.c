// 验证子进程是父进程的不完全副本
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int global = 1;



int main()
{
    int local = 2;  
    int* heap = malloc(sizeof(int));
    *heap = 3;
    printf("%d进程:%p->%d,%p->%d,%p->%d\n",getpid(),
    &global,global,&local,local,heap,*heap);

    // 创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        return -1;
    }
    // 子进程代码
    if(pid == 0)
    {
        printf("%d子进程:%p->%d,%p->%d,%p->%d\n",getpid(),
        &global,++global,&local,++local,heap,++*heap);
      
        return 0;
    }
    // 父进程代码
    sleep(1);
    printf("%d父进程:%p->%d,%p->%d,%p->%d\n",getpid(),
    &global,global,&local,local,heap,*heap);
    return 0;
}
