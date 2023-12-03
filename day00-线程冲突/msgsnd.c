#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <errno.h>

#define TEXT_SIZE  512
static int msqid;   // 创建的消息队列
int is_msg_deleted = 0;  // 判断消息队列是否删除

struct msgbuf
{
    long mtype;
    int  status;
    char time[20] ;
    char mtext[TEXT_SIZE];
};

struct msgbuf buf;
struct msgbuf buf1;
int flag;
int sendlength, recvlength;

char* getxtsj()
{ 
    time_t tv ;
    struct tm *tmp ;
    static char buf[20] ;
    tv = time(0) ;
    tmp = localtime(&tv) ;
    sprintf(buf,"%02d:%02d:%02d",tmp->tm_hour, tmp->tm_min,tmp->tm_sec);
    return  buf ;
}

// 线程过程函数
void* send_msg1(void *arg)
{
    while(1)
    {
        buf1.mtype = 1 ; 
        buf1.status = 9 ;
        strcpy(buf1.time, getxtsj()) ;
        sprintf(buf1.mtext, "我是子线程%ld!",syscall(SYS_gettid)) ;    // 包含线程信息
        printf("%ld线程:发送[%s]\n",syscall(SYS_gettid), buf1.mtext);  
        sendlength = sizeof(struct msgbuf) - sizeof(long);
        if(is_msg_deleted == 1)
        {
            printf("%ld线程:消息队列被删除!\n",syscall(SYS_gettid));
            break;
        }

        flag = msgsnd( msqid, &buf1, sendlength , 0 ) ;
        if (flag < 0)
        {

            perror("send message error") ;
            return NULL;
        }

        sleep(1);
    }
    return (void* )1;
}

void* send_msg2(void *arg)
{

    while(1)
    {
        buf.mtype = 2 ; 
        buf.status = 9 ;
        strcpy(buf.time, getxtsj()) ;
        sprintf(buf.mtext, "我是子线程%ld!",syscall(SYS_gettid)) ;    // 包含线程信息
        printf("%ld线程:发送[%s]\n",syscall(SYS_gettid), buf.mtext);  
        sendlength = sizeof(struct msgbuf) - sizeof(long);
        if(is_msg_deleted == 1)
        {
            printf("%ld线程:消息队列被删除!\n",syscall(SYS_gettid));
            break;
        }
        flag = msgsnd(msqid, &buf, sendlength , 0) ;
        if ( flag < 0 )
        {
            
            perror("send message error") ;
            return NULL;
        }

        sleep(1);
    }
    return (void* )1;
}


int main(int argc, char **argv)
{
    
    int key ;  
    key = ftok(".", 0x01 ) ;
    if ( key < 0 )
    {
        perror("ftok key error") ;
        return -1 ;
    }
    // 取消缓冲区
    fflush(stdout); // 取消缓冲区并立即输出内容

    printf("%ld线程:创建消息队列\n",syscall(SYS_gettid));
    msqid = msgget(key, 0600 | IPC_CREAT) ;
    if (msqid < 0 )
    {
        perror("create message queue error") ;
        return -1 ;
    }

    printf("%ld线程:我是主线程,开始创建子线程\n",syscall(SYS_gettid));   
    // 创建线程
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,send_msg1,NULL);
    pthread_create(&tid2,NULL,send_msg2,NULL);


    // 阻塞主线程
    getchar();
    printf("销毁消息队列%d\n",msqid);
    if(msgctl(msqid,IPC_RMID,NULL) == -1)
    {
        perror("msgctl error");
        return -1;
    }
    is_msg_deleted = 1;

    // 汇合线程
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    


    return 0 ;
}


