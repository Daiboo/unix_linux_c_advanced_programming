// 从共享内存读入数据
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>



int main(void)
{   
    // 合成键
    printf("%d进程:合成键\n",getpid());
    key_t key = ftok(".", 123);
    if(key == -1)
    {
        perror("ftok error");
        return -1;
    }
    // 获取共享内存
    printf("%d进程:获取共享内存\n",getpid());
    int shmid = shmget(key, 0, 0);  // 获取
    if(shmid == -1)
    {
        perror("shmget error");
        return -1;
    }
    

    // 加载共享内存
    printf("%d进程:加载共享内存\n",getpid());
    char* start = shmat(shmid, NULL, 0);
    if(start == (void*)-1)
    {
        perror("shmat error");
        return -1;
    }

    // 读取数据
    getchar();
    printf("%d进程:读取数据\n",getpid());
    printf("%s\n",start);

    // 卸载共享内存
    printf("%d进程:卸载共享内存\n",getpid());
    if(shmdt(start) == -1)
    {
        perror("shmdt");
        return -1;
    }

    printf("%d进程:大功告成\n",getpid());

    return 0;
}