// 分离线程
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

// 线程处理函数
void* pthread_fun(void* arg)
{
    for(int i = 0; i < 100; i++)
    {
        putchar('-'); 
        usleep(50000);  // 0.05s
    }
    return NULL;
}

int main(void)
{
    setbuf(stdout, NULL);  // 关闭输出缓冲区
    pthread_t tid;
    pthread_create(&tid, NULL, pthread_fun, NULL);
    // 设置分离线程
    pthread_detach(tid);

    // int ret = pthread_join(tid, NULL);
    // if(ret)
    // {
    //     fprintf(stderr, "pthread_join %s\n",strerror(ret));
    //     return -1;
    // }


    for(int i = 0; i < 100; i++)
    {
        putchar('*');
        usleep(50000);  // 0.05s
    }

    return 0;
}