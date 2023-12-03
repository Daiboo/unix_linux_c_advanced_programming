// 互斥锁
#include <stdio.h>
#include <unistd.h>   
#include <pthread.h>  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *ThreadFun(void *arg)//线程函数
{  
    if(pthread_mutex_lock(&mutex) != 0)
    {
        printf("Mutex lock failed!\n");
        return NULL;
    }
    int num=5;
    while(num--){
        printf("Accessing %s!\n", (char*)arg);
        sleep(1);
    }

    if(pthread_mutex_unlock(&mutex) != 0)
    {
        printf("Mutex unlock failed!\n");
        return NULL;
    }
    return NULL;
}

int main()
{
    int res;
    char * url1 = "http://thread1.net";
    char * url2 = "http://thread2.net";
    void* state=NULL;
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
    // pthread_detach(tid1);//将线程1和主线程分离,操作系统会自动释放
    // pthread_detach(tid2);//将线程2和主线程分离
    pthread_join(tid1,state);//令主线程等到线程1执行完成
    pthread_join(tid2,state);//令主线程等到线程2执行完成
    printf("主线程继续其他任务！\n");
    sleep(5);
    return 0;
}
