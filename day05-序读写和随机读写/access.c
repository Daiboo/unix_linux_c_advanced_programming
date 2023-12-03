// 访问测试
#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[])
{   
    // ./a.out hello.txt
    // 文件:hello.txt存不存在
    // 文件:hello.txt 可读,可写,可执行
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return -1;
    }
    
    printf("文件:%s",argv[1]);
    if(access(argv[1], F_OK) == -1)
    {
        printf("文件不存在\n");
    }
    else
    {
        // 能读否
        if(access(argv[1], R_OK) == -1)
        {
            printf("不可读,");
        }
        else
        {
            printf("可读,");
        }
        // 能写否
        if(access(argv[1], W_OK) == -1)
        {
            printf("不可写,");
        }
        else
        {
            /* code */
            printf("可写,");
        }
        // 能执行否
        if(access(argv[1], X_OK) == -1)
        {
            printf("不可执行\n");
        }
        else
        {
            printf("可执行\n");
        }
        
    }
    
    return 0;
}
