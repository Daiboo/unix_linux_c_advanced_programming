// system函数
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{   
    int s = system("./new hello 123");  // 成功返回new终止后的结束状态
    if(s == -1)
    {
        perror("system");
        exit(-1);
    }
    if(WIFEXITED(s))
    {
        printf("正常终止:%d\n",WEXITSTATUS(s));
    }
    else
    {
        printf("异常终止:%d\n",WTERMSIG(s));
    }
  

    return 0;
} 