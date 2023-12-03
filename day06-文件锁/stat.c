#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<time.h>
#include<fcntl.h>



// 文件元数据mode_t 转换类型和权限
// s.st_mode -> -rw-rw-r--
char* mtos(mode_t m)
{
    static char s[11];
    if(S_ISDIR(m)) strcpy(s, "d");
    else if(S_ISSOCK(m)) strcpy(s, "s");
    else if (S_ISCHR(m)) strcpy(s, "c");
    else if (S_ISBLK(m)) strcpy(s, "c");
    else if (S_ISLNK(m)) strcpy(s, "l");
    else if (S_ISFIFO(m)) strcpy(s, "p");
    else strcpy(s, "-");

    strcat(s, m & S_IRUSR ? "r" : "-");
    strcat(s, m & S_IWUSR ? "w" : "-");
    strcat(s, m & S_IXUSR ? "x" : "-");
    strcat(s, m & S_IRGRP ? "r" : "-");
    strcat(s, m & S_IWGRP ? "w" : "-");
    strcat(s, m & S_IXGRP ? "x" : "-");
    strcat(s, m & S_IROTH ? "r" : "-");
    strcat(s, m & S_IWOTH ? "w" : "-");
    strcat(s, m & S_IXOTH ? "x" : "-");
    return s;
}

// 转化time_t类型 为 年月日时分秒
char* ttos(time_t t)
{
    static char time[26];
    struct tm *tm = localtime(&t);
    sprintf(time, "%04d-%02d-%02d %02d:%02d:%02d", tm->tm_year+1900,
    tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec
    );
    return time;
}



int main(int argc, char *argv[])
{
    // ./a.out hello.txt
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return -1;
    }
    // 获取文件的元数据
    struct stat sb;  // 用来输出元数据信息
    if(stat(argv[1], &sb) == -1)
    {
        perror("stat");
        return -1;
    }

    // 输出sb
    printf("File: %s\n", argv[1]);
    printf("Size: %ld\n", sb.st_size);
    printf("Mode: %s\n", mtos(sb.st_mode));
    printf("Last modified: %s\n", ttos(sb.st_mtime));
    return 0;
    
}