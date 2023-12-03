#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM 5               
int queue[NUM];                                     //全局数组实现环形队列
sem_t blank_number, product_number,mutexqu;                 //空格子信号量, 产品信号量
//生产者
void *producer(void *arg)
{
    int i = 0;
    while (1) 
    {
        sem_wait(&blank_number);                    //生产者将空格子数--,为0则阻塞等待
        sem_wait(&mutexqu);                         //加上互斥锁
        queue[i] = rand() % 1000 + 1;               //生产一个产品
        printf("Produce---%d\n", queue[i]);  
        sem_post(&mutexqu);                         //解开互斥锁      
        sem_post(&product_number);                  //将产品数++
        i = (i+1) % NUM;                            //借助下标实现环形
        sleep(rand()%3);
    }
}
//消费者
void *consumer(void *arg)
{
    int i = 0;
    while (1) {
        sem_wait(&product_number);                  //消费者将产品数--,为0则阻塞等待
        sem_wait(&mutexqu);                         //加上互斥锁 
        printf("Consume---%d\n", queue[i]);
        queue[i] = 0;                               //消费一个产品 
        sem_post(&mutexqu);                         //解开互斥锁   
        sem_post(&blank_number);                    //消费掉以后,将空格子数++
        i = (i+1) % NUM;
        sleep(rand()%3);
    }
}
int main(int argc, char *argv[])
{
    pthread_t pid, cid;
    sem_init(&blank_number, 0, NUM);                //初始化空格子信号量为5
    sem_init(&product_number, 0, 0);                //产品数为0
    sem_init(&mutexqu, 0, 1);                //产品数为0
	//创建生产者和消费者线程
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
	//等待线程结束
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
	//销毁信号量
    sem_destroy(&blank_number);
    sem_destroy(&product_number);
    sem_destroy(&mutexqu);

    return 0;
}

