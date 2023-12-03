// 线程过程函数的参数
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define PI 3.14159
// 线程过程函数,计算园的面积 s = pi * r * r
void* area(void* arg)
{
    double r = *(double*)arg;
    *(double*)arg = PI * r * r;
    
    return NULL;
}

int main(void)
{
    // 创建线程
    pthread_t tid;
    double r = 10;
    pthread_create(&tid, NULL, area, (void*)&r);
    sleep(1); 
    printf("圆的面积是%lg\n",r);

    return 0;
}