// 线程的创建
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


void* thread_func(void *arg)
{
    printf("%lu线程:我是子线程 --> %s\n",pthread_self(),(char*)arg);
    return NULL;
}



int main(void)
{
    printf("%lu线程:我是主线程,我要创建子线程\n",pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,thread_func,"daiboo!");
    if(ret)
    {
        fprintf(stderr,"pthread_create error:%s\n",strerror(ret));
        return -1;
    }
    sleep(1);
    printf("%lu线程:我是主线程,我创建了%lu线程\n",pthread_self(),tid);
    
    return 0;
}