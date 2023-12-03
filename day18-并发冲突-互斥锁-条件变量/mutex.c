// 并发冲突
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 锁

// 线程过程函数
int g_cnt;
void* thread_func(void* arg)
{   // 枷锁
    pthread_mutex_lock(&mutex);
    for(int i = 0; i < 100000; i++)
    {
        g_cnt++;
    }
    // 解锁 
    pthread_mutex_unlock(&mutex);
    return NULL;
}


int main(void)
{
    // 线程

    pthread_t tid1,tid2;
    pthread_create(&tid1, NULL, thread_func, NULL);
    pthread_create(&tid2, NULL, thread_func, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("g_cnt = %d\n", g_cnt);

    return 0;
}