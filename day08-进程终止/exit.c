// 进程的终止
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// 退出处理函数
void doit(void)
{
    printf("遗言也没啥可说的\n");
}

// 可传参的退出处理函数
void doit2(int status, void* arg)
{
    // status 进程的退出码
    printf("status = %d\n",status);
    printf("arg = %s\n",(char*)arg);
}

int hahaha(void)
{
    printf("hahaha\n");
    // exit(0); 
    // _exit(10); 
    return 10;
}

int main()
{
    // 注册退出处理函数
    atexit(doit);
    on_exit(doit2,"baibaiba");
    printf("函数返回%d\n",hahaha());
    return 0;
}