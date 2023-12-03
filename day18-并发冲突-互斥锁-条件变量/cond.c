// 条件变量解决消费者生产者问题
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
char g_storage[10];  // 仓库
int g_stock = 0;  // 当前的库存量 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pcond = PTHREAD_COND_INITIALIZER;  // 生产者卧室
pthread_cond_t ccond = PTHREAD_COND_INITIALIZER;  // 消费者卧室

// 显示当前库存情况  生产者:ABC <-- D 消费者:ABC --> D
void show(char* who, char* op, char prod)
{
    printf("%s",who);
    for(int i = 0; i < g_stock; i++)
    {
        printf("%c",g_storage[i]);
    }
    printf("%s%c\n",op,prod);
}


// 生产者线程
void* producer(void *arg)
{
    char* who = (char*)arg;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        // 判断
        if(g_stock == 10)
        {
            printf("%s:满仓\n",who);
            pthread_cond_wait(&pcond, &mutex);  // 睡入
        }
    
        // 生产
        char prob = 'A' + rand() % 26;  // 随机字符
        show(who, " <-- ", prob);
        g_storage[g_stock++] = prob;

        // 唤醒消费者
        pthread_cond_signal(&ccond);
        //解锁
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 100 * 10000);
        // sleep(rand() % 3);
    }
}
// 消费者线程
void* consumer(void *arg)
{
    char* who = (char*)arg;
    while(1)
    {
        // 枷锁
        pthread_mutex_lock(&mutex);
        // 判断
        if(g_stock == 0)
        {
            printf("%s:空仓\n",who);
            pthread_cond_wait(&ccond, &mutex);
        }
        // 消费
        char prob = g_storage[--g_stock];
        show(who, " --> ", prob);
        // 唤醒生产者
        pthread_cond_signal(&pcond);
        // 解锁
        pthread_mutex_unlock(&mutex);
        usleep(rand() % 100 * 10000);
    }
}

int main(void)
{
    // 创建线程
    pthread_t pid, cid;
    pthread_create(&pid, NULL, producer, "生产者");
    pthread_create(&cid, NULL, consumer, "消费者");

    getchar();  // 主线程阻塞

    return 0;
}
