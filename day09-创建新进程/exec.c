// exec家族函数
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    printf("%d进程:我要变身了,哈哈哈\n",getpid());
    // if(execl("/bin/ls","ls","-l",NULL) == -1)  // 变身成进程ls
    // {
    //     perror("execl");
    //     exit(-1);
    // }
    // if(execlp("ls","ls","-l",NULL) == -1)
    // {
    //     perror("execlp");
    //     exit(-1);
    // }
    // if(execlp("new","new","hello",NULL) == -1)
    // {
    //     perror("execlp");
    //     exit(-1);
    // }
    char* argv[] = {"new","hello","123","456",NULL};
    char* envp[] = {
        "NAME=daiboo",
        "AGE=18",
        "FOOD=铁锅炖大鹅",
        NULL
    };
    if(execve("./new",argv,envp) == -1)
    {
        perror("execve");
        exit(-1);
    }
    // if(execle("./new","new","hello","123",NULL,envp) == -1)
    // {
    //     perror("execle");
    //     exit(-1);
    // }

    // 变身成功后,后面的代码执行不到的
    printf("%d进程:我变身完成了,嘿嘿嘿\n",getpid());

    return 0;
}