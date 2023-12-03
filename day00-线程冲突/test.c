#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>

#define MAX_MSG_SIZE 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

int msg_id; // 全局变量，在主线程和子线程之间共享

void* threadFunc(void* arg) {
    struct msg_buffer msg;
    int counter = 0;

    while (1) {
        msg.msg_type = 1;
        sprintf(msg.msg_text, "Message %d", counter++);

        if (msgsnd(msg_id, &msg, sizeof(msg), 0) == -1) {
            if(errno == EIDRM)
            {
                printf("msg_id is removed, exit\n");
                break;
            }
            else
            {            
                perror("msgsnd");
                exit(1);
            }
            

        }

        sleep(1); // 为了演示目的，每秒发送一条消息


    }

    return NULL;
}

int main() {
    key_t key;
    pthread_t thread;
    struct msg_buffer msg;

    // 创建一个唯一的key
    key = ftok("msg_queue_example", 65);

    // 创建消息队列
    msg_id = msgget(key, 0666 | IPC_CREAT);
    if (msg_id == -1) {
        perror("msgget");
        exit(1);
    }

    // 创建线程
    if (pthread_create(&thread, NULL, threadFunc, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }

    printf("Press Enter to destroy message queue: ");
    getchar();

    // 销毁消息队列
    if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    // 等待线程结束
    pthread_join(thread, NULL);

    printf("Message queue destroyed. Exiting program.\n");

    return 0;
}