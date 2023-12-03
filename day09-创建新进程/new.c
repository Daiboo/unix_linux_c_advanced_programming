#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[],char* envp[]) 
{
    printf("PID=%d",getpid());
    printf("命令行参数:\n");
    for(char** pp = argv; *pp; pp++)
    {
        printf("%s\n",*pp);
    }
    printf("环境变量:\n");
    for(char** pp = envp; *pp; pp++)
    {
        printf("%s\n",*pp);
    };
    printf("-----------------------------\n");

    return 0;
}