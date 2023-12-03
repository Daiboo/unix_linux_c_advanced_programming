// 线程的汇入
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define PI 3.1415926
// 线程处理函数,计算圆的面积
void* area(void* arg)
{
    double r = *(double*)arg;
    static double area;
    area = PI * r * r;
    sleep(5);
    return &area;
}
int main(void)
{
    double r = 10;
    pthread_t tid;
    pthread_create(&tid, NULL, area, &r);
    double* area;  // 接线程的返回值
    pthread_join(tid, (void**)&area);
    printf("圆的面积是%lg\n",*area);

    return 0;
}