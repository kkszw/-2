#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct PCB{//PCB结构体
    char name[8];
    int begin;
    int size;
    int page_table[10][3];
    int page_num;
    struct PCB *left;
    struct PCB *right;
}Mypcb;
Mypcb pcb;
int block_size,block_capacity;//块的容量和大小
int bitmap[8][8];//位示图
int mystack[3],top=0;
int opt[20],sus=0;//OPT算法存储数组
void bitmapSrand()//位示图随机产生
{
    srand(time(NULL));
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            bitmap[i][j]=rand()%2;
        }
    }
}
void outBitMap()
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
            printf("%d ",bitmap[i][j]);
        printf("\n");
    }
}
void CreateProcess()//创建进程
{
    int temp,top=0;
    printf("请输入进程的名称：");
    scanf("%s",pcb.name);
    printf("请输入进程的大小:");
    scanf("%d",&pcb.size);
    if(pcb.size>block_capacity*1024)
    {
        printf("进程大小不能大于块的容量!\n");
        return;
    }
    temp=pcb.size%(block_size*1024);
    if(temp==0)
        pcb.page_num=pcb.size/(block_size*1024);
    else
        pcb.page_num=pcb.size/(block_size*1024)+1;
    for(int i=0;i<pcb.page_num;i++)
    {
        pcb.page_table[i][0]=-1;
        pcb.page_table[i][1]=0;
    }
}
void KillProcess()//结束进程
{
    int i,j;
    for(int n=0;n<pcb.page_num;n++)
    {
        if(pcb.page_table[n][0]!=-1)
        {
            i=pcb.page_table[n][0]/8;
            j=pcb.page_table[n][0]%8;
            bitmap[i][j]=0;
        }
    }
    Mypcb pcb1;
    pcb=pcb1;
}
int findFirstZero()//在位示图中找到第一个0
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(bitmap[i][j]==0)
            {
                bitmap[i][j]=1;//位示图从0变1
                return i*8+j;
            }
        }
    }
}
void push(int a)
{
    if(top==3)
    {
        int b[3];
        b[0]=mystack[1];
        b[1]=mystack[2];
        b[2]=a;
        for(int i=0;i<3;i++)
            mystack[i]=b[i];
    }
    else
    {
        mystack[top]=a;
        top++;
    }

}
void showStack()
{
    printf("栈：");
    for(int i=0;i<top;i++)
    {
        printf("%d ",mystack[i]);
    }
    printf("\n");
}
void showPageTable()
{
    printf("页号 块号 状态\n");
    for(int i=0;i<pcb.page_num;i++)
    {
        printf("%-4d%-4d%-4d\n",i,pcb.page_table[i][0],pcb.page_table[i][1]);
    }
}
void FIFO()
{
    top=0;
    sus=0;
    int a=1,logic_page_num,page_address;
    int num=0,lackNum=0;//操作次数,缺页次数
    while(a!=-1)
    {
        printf("请输入逻辑地址:");
        scanf("%d",&a);
        if(a==-1)
            break;
        logic_page_num=a/(block_size*1024);
        page_address=a%(block_size*1024);
        if(logic_page_num>=pcb.page_num)
        {
            printf("超出范围！\n");
            return;
        }
        else
        {
            opt[sus]=logic_page_num;
            sus++;
            num++;
            if(pcb.page_table[logic_page_num][1]!=1)//缺页
            {
                if(top!=3)//栈没满
                {
                    pcb.page_table[logic_page_num][0]=findFirstZero();
                    pcb.page_table[logic_page_num][1]=1;
                    push(logic_page_num);
                }
                else//栈满了替换出去
                {
                    pcb.page_table[logic_page_num][0]=pcb.page_table[mystack[0]][0];
                    pcb.page_table[logic_page_num][1]=1;
                    pcb.page_table[mystack[0]][1]=0;
                    pcb.page_table[mystack[0]][0]=-1;
                    push(logic_page_num);
                }
                lackNum++;
            }
        }
        printf("物理地址为:%d  页内地址为:%d\n",pcb.page_table[logic_page_num][0]*block_size*1024+page_address,page_address);
        printf("缺页次数:%d 操作次数:%d 缺页率:%.2f%%\n",lackNum,num,(lackNum/(num*1.0))*100);
        showStack();
        showPageTable();
    }
}
void swapStack(int a)
{
    if(top==3)
    {
        for(int i=0;i<2;i++)
        {
            if(mystack[i]==a)
            {
                for(int j=i;j<2;j++)
                {
                    mystack[j]=mystack[j+1];
                    mystack[j+1]=a;
                }
            }
        }
    }
}
void LRU()
{
    top=0;
    sus=0;
    int a=1,logic_page_num,page_address;
    int num=0,lackNum=0;//操作次数,缺页次数
    while(a!=-1)
    {
        printf("请输入逻辑地址:");
        scanf("%d",&a);
        if(a==-1)
            break;
        logic_page_num=a/(block_size*1024);
        page_address=a%(block_size*1024);
        if(logic_page_num>=pcb.page_num)
        {
            printf("超出范围！\n");
            return;
        }
        else
        {
            opt[sus]=logic_page_num;
            sus++;
            num++;
            if(pcb.page_table[logic_page_num][1]!=1)//缺页
            {
                if(top!=3)//栈没满
                {
                    pcb.page_table[logic_page_num][0]=findFirstZero();
                    pcb.page_table[logic_page_num][1]=1;
                    push(logic_page_num);
                }
                else//栈满了替换出去
                {
                    pcb.page_table[logic_page_num][0]=pcb.page_table[mystack[0]][0];
                    pcb.page_table[logic_page_num][1]=1;
                    pcb.page_table[mystack[0]][1]=0;
                    pcb.page_table[mystack[0]][0]=-1;
                    push(logic_page_num);
                }
                lackNum++;
            }
            else//命中
            {
                swapStack(logic_page_num);
            }
        }
        printf("物理地址为:%d  页内地址为:%d\n",pcb.page_table[logic_page_num][0]*block_size*1024+page_address,page_address);
        printf("缺页次数:%d 操作次数:%d 缺页率:%.2f%%\n",lackNum,num,(lackNum/(num*1.0))*100);
        showStack();
        showPageTable();
    }
}
int findStack(int goal)//查找数组中是否存在该元素
{
    for(int i=0;i<3;i++)
    {
        if(mystack[i]==goal)
            return 1;
    }
    return 0;
}
void OPT()
{
    int goal1,goal2;
    top=0;
    int lackNum=0;//缺页次数
    for(int i=0;i<sus-2;i++)
    {
        if(top==3)
        {
            if(!findStack(opt[i]))//缺页
            {
                lackNum++;
                for(int j=0;j<3;j++)
                {
                    if(mystack[j]!=opt[i+1]&&mystack[j]!=opt[i+2])
                    {
                        mystack[j]=opt[i];
                        break;
                    }
                }
            }
        }
        else
            push(opt[i]);
        printf("%d ",opt[i]);
        showStack();
    }
    if(!findStack(opt[sus-2]))//缺页
    {
        for(int i=0;i<3;i++)
        {
            if(mystack[i]!=opt[sus-1])
            {
                mystack[i]=opt[sus-1];
                break;
            }
        }
    }
    printf("%d ",opt[sus-2]);
    showStack();
    if(!findStack(opt[sus-1]))
    {
        push(opt[sus-1]);
    }
    printf("%d ",opt[sus-1]);
    showStack();
    printf("缺页次数:%d 操作次数:%d 缺页率:%.2f%%\n",lackNum,sus,(lackNum/(sus*1.0))*100);
}
void Menu()
{
    int a=1;

    while(a!=0)
    {
        printf("1.创建进程\n");
        printf("2.结束进程\n");
        printf("3.展示位示图\n");
        printf("4.FIFO\n");
        printf("5.LRU\n");
        printf("6.OPT\n");
        printf("0.结束程序\n");
        printf("请输入你的选择:");
        scanf("%d",&a);
        switch(a)
        {
        case 0:
            break;
        case 1:
            CreateProcess();
            break;
        case 2:
            KillProcess();
            break;
        case 3:
            outBitMap();
            break;
        case 4:
            FIFO();
            break;
        case 5:
            LRU();
            break;
        case 6:
            OPT();
            break;
        default:
            printf("请重新选择!\n");
        }
    }
}
void main()
{
    bitmapSrand();
    printf("请输入块容量:");
    scanf("%d",&block_capacity);
    printf("请输入块的大小:");
    scanf("%d",&block_size);
    Menu();

}
/*
8
1
3
1
1
8000
4
7169
1023
1025
2049
2050
-1
3
2
1
2
8000
5
7169
1023
1025
2049
2050
1022
-1
6

*/
