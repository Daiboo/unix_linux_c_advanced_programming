#include<stdio.h>
#include<stdlib.h>
int main()
{
    int  n ,m,t,w,flag1=1,flag2=1,flag4=1,flag5=1;
    int*Available,*Request,*Finish;
    int **Allocation,**Max,**Need,**Work;
    FILE*fp;
    fp=fopen("./data1.txt","r");                   //打开.txt文件
    fscanf(fp,"%d",&n),fscanf(fp,"%d",&m);                 //赋值.txt文件的数值，n*m二维数组
    Available = (int*)malloc(sizeof(int)*m);                                  //开动态一维数组
    for(int i=0;i<m;i++)                                                         //给一维数组赋值
        fscanf(fp,"%d",&Available[i]);
    Allocation= (int**)malloc(sizeof(int*)*n);               //开动态二维数组
    Max= (int**)malloc(sizeof(int*)*n); 
    Need= (int**)malloc(sizeof(int*)*n); 
    for (int i = 0; i < n; i++)
    {
        Allocation[i] = (int*)malloc(sizeof(int)*m);
        Max[i] = (int*)malloc(sizeof(int)*m);
        Need[i] = (int*)malloc(sizeof(int)*m);
    }


    for(int i=0;i<n;i++)                       //给二维数组赋值
    {
        fscanf(fp,"%d",&t);               //t为进程编号
        for(int j=0;j<m;j++)
            fscanf(fp,"%d",&Allocation[t][j]);
        for(int  j=0;j<m;j++)
            fscanf(fp,"%d",&Max[t][j]);
        for(int j=0;j<m;j++)
            Need[i][j]=Max[i][j]-Allocation[i][j];
    }
    // 打印
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            printf("%d ",Allocation[i][j]);
        printf("\n");
    }
    printf("\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            printf("%d ",Max[i][j]);
        printf("\n");
    }
    printf("\n");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            printf("%d ",Need[i][j]);
        printf("\n");
    }
    printf("\n");


     for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
                Available[i]-=Allocation[j][i];
    fscanf(fp,"%d",&w);             //w为发出请求的进程的编号
    Request = (int*)malloc(sizeof(int)*m);
    for(int i=0;i<m;i++)
            fscanf(fp,"%d",&Request[i]);
    for(int i=0;i<m;i++)
        if(Request[i]>Need[w][i])
            flag1=0; 
    if(!flag1)                       //flag1用来判断Request是否小于等于Need
        printf("Flase");
    else {
        for(int i=0;i<m;i++)
            if(Request[i]>Available[i])
                flag2=0; 
        if(!flag2)                          //flag2用来判断Request是否小于等于Available
            printf("p %d 等待",w);
        else {
            for(int i=0;i<m;i++)             //假设系统把申请的资源分给进程
                {
                    Available[i]-=Request[i];
                    Allocation[w][i]+=Request[i];
                    Need[w][i]-=Request[i];
                }
            Work= (int**)malloc(sizeof(int*)*n);
            for (int i = 0; i < n; i++)
                Work[i] = (int*)malloc(sizeof(int)*m);
            Finish = (int*)malloc(sizeof(int)*n);
            for(int i=0;i<n;i++)
                Finish[i]=0;                     //fnish[i]=0代表fnish[i]=flase，fnish[i]=1代表fnish[i]=true
            for(int i=0;i<n;i++)
                for(int j=0;j<m;j++)
                        Work[i][j]=Available[j];
            while(flag5)               //安全性算法，flag5用来判断系统是否已经完成安全性判断
                {
                    for(int i=0;i<n;i++)                //每次从0号进程开始搜索是否有满足条件的进程
                        {
                            int flag3=1;                             //flag3用来判断Need是否小于等于Work
                            for(int j=0;j<m;j++)
                                   if(Need[i][j]>Work[i][j])
                                        flag3=0;
                            if(!Finish[i]&&flag3)            //找到满足Finish[i]=false且Need小于等于Work
                            {
                                Finish[i]=1;                            //置Finish[i]=true
                                for(int j=0;j<n;j++)                   //释放进程所占全部资源
                                    for(int k=0;k<m;k++)
                                        if(j!=i&&!Finish[j])
                                               Work[j][k]=Work[i][k]+Allocation[i][k];
                                break;
                            }
                            else if(i==n-1)                        //没有找到满足条件的并且是最后一个进程
                            {
                                    for(int j=0;j<n;j++)
                                        if(!Finish[j])
                                            flag4=0; 
                                    if(flag4)                       //flag4用来判断是否所有进程都是Finish[i]=true
                                    {
                                        printf("系统处于安全状态\n");
                                        printf("Work:\n");
                                        for(int j=0;j<n;j++)              //输出安全状态下，系统把申请的资源分配给进程资源的情况
                                            {
                                                for(int k=0;k<m;k++)
                                                printf("%4d",Work[j][k]);
                                                printf("\n");
                                            }
                                        printf("Alllcation:\n");
                                        for(int j=0;j<n;j++)
                                            {
                                                for(int k=0;k<m;k++)
                                                printf("%4d",Allocation[j][k]);
                                                printf("\n");
                                            }
                                        printf("Need:\n");
                                        for(int j=0;j<n;j++)
                                            {
                                                for(int k=0;k<m;k++)
                                                printf("%4d",Need[j][k]);
                                                printf("\n");
                                            }
                                        printf("Available:\n");
                                        for(int j=0;j<m;j++)
                                            printf("%4d",Work[n-1][j]+Allocation[n-1][j]);
                                        flag5=0;
                                    }
                                    else{
                                        printf("系统处于不安全状态\n");
                                        for(int j=0;j<m;j++)             //系统不安全，还原资源分配给进程前的情况
                                            {
                                                Available[j]+=Request[j];
                                                Allocation[w][j]-=Request[j];
                                                Need[w][j]+=Request[j];
                                            }
                                        flag5=0;
                                    }
                            }
                        }
                }
        }
    }
}
