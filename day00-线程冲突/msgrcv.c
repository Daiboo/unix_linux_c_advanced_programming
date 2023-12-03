#include <stdio.h> 
#include <string.h> 
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>

#define TEXT_SIZE  512
struct msgbuf
{
    long mtype ;
    int  status ;
    char time[20] ;
    char mtext[TEXT_SIZE] ;
}  ;
 
int msqid ;
struct msqid_ds info ;
struct msgbuf buf;
struct msgbuf buf1 ;
int flag ;
int recvlength;
int key ;
int mtype ;

// 线程过程函数
void* rcv_msg1(void *arg)
{
    while(1)
    {
        recvlength = sizeof(struct msgbuf) - sizeof(long) ;
        memset(&buf, 0x00, sizeof(struct msgbuf));
        mtype = 1;  
        flag = msgrcv(msqid, &buf, recvlength ,mtype,0) ;
        
        if(flag < 0)
        {   
            if(errno == EIDRM)
            {
                printf("%ld线程:消息队列已经被删除\n",syscall(SYS_gettid));
                break;
            }
            else
            {
                perror("recv message error\n") ;
                return NULL ;
            }
        }
        printf("%ld线程:type=%ld,time=%s, message=%s\n", syscall(SYS_gettid),buf.mtype, buf.time,  buf.mtext) ;
    }
    return (void* )1;
}
void* rcv_msg2(void *arg)
{
    while(1)
    {
        recvlength = sizeof(struct msgbuf) - sizeof(long) ;
        memset(&buf1, 0x00, sizeof(struct msgbuf));
        mtype = 2;  
        flag = msgrcv(msqid, &buf1, recvlength ,mtype,0) ;
        
        if(flag < 0)
        {   
            if(errno == EIDRM)
            {
                printf("%ld线程:消息队列已经被删除\n",syscall(SYS_gettid));
                break;
            }
            else
            {
                perror("recv message error\n") ;
                return NULL ;
            }
        }
        printf("%ld线程:type=%ld,time=%s, message=%s\n", syscall(SYS_gettid),buf1.mtype, buf1.time,  buf1.mtext) ;
    }
    return (void* )1;
}
int main(int argc, char **argv)
{
    
    key = ftok(".", 0x01) ;
    if (key < 0)
    {
        perror("ftok key error") ;
        return -1 ;
    }
    
    msqid = msgget(key, 0) ;
    if (msqid < 0 )
    {
        perror("get ipc_id error") ;
        return -1 ;
    }
    // 创建线程
    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,rcv_msg1,NULL);
    pthread_create(&tid2,NULL,rcv_msg2,NULL);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    return 0 ;
}

