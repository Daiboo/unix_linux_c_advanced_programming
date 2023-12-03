// 采用信号量解决线程冲突问题
#include <stdio.h>
#include <unistd.h>   
#include <pthread.h>  
#include <semaphore.h>
sem_t mutex;  // 互斥锁

void *ThreadFun(void *arg)//线程函数
{  
    sem_wait(&mutex);   // 占用资源
    int num=5;
    while(num--){
        printf("Accessing %s!\n", (char*)arg);
        sleep(1);
    }
    sem_post(&mutex);  // 释放
    return NULL;
}

int main()
{
    int res;
    char * url1 = "http://thread1.net";
    char * url2 = "http://thread2.net";
    void* state=NULL;
    // 初始化信号量
    sem_init(&mutex, 0, 1);   // 初始化为1

    pthread_t tid1,tid2;//定义两个表示线程的变量（标识符）
    //创建用户线程1
    res = pthread_create(&tid1, NULL, ThreadFun, (void*)url1);
    if (res != 0) {
        printf("线程创建失败");
        return 0;
    }
    //创建用户线程2
    res = pthread_create(&tid2, NULL, ThreadFun,(void*)url2);
    if (res != 0) {
        printf("线程创建失败");
        return 0;
    }
    // pthread_detach(tid1);//将线程1和主线程分离
    // pthread_detach(tid2);//将线程2和主线程分离
    pthread_join(tid1,state);//令主线程等到线程1执行完成
    pthread_join(tid2,state);//令主线程等到线程2执行完成
    printf("主线程继续其他任务！\n");
    sleep(5);
    
    sem_destroy(&mutex);

    return 0;
}
