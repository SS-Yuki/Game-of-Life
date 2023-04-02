#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <windows.h>
#include <conio.h>
#include <io.h>


#define HELP 13404          // hash value for \h
#define PRINT 14428         // hash value for \p
#define RANDOM 14044        // hash value for \m
#define CATALOG 12764       // hash value for \c
#define DESIGN 12892        // hash value for \d
#define QUIT 14556          // hash value for \q
#define LOAD 13916          // hash value for \l
#define SAVE 14812          // hash value for \s
#define GENERATE 13276      // hash value for \g
#define RUN 14684           // hash value for \r
#define EXIT 13020          // hash value for \e
#define END 1652581         // hash value for end


void updatewithoutinput();      //与用户输入无关的步骤
void updatewithinput();         //与用户输入相关的步骤
long hash(char *a);             //哈希值计算
void getInput(char *buf);       //获取字符串输入
void help_display();            //打印命令提示
void print_map();               //打印当前地图
void random_map();              //生成随机地图
void catalog_display();         //打印文件目录
void design_map();              //设计地图
void load_map();                //导入地图
void save_map();                //保存地图
int alive_count(int p, int q);  //周围存活细胞计算
void step_map();                //单步运行
void auto_game();               //自动运行
void malloc_array();            //分配存储空间
void free_array();              //释放存储空间


int x, y;           //x为地图行数，y为地图列数
int **game_array;   //声明二级指针


int main()      //主函数
{
    updatewithoutinput();
    updatewithinput();
    return 0;
}


void updatewithoutinput()
{
    random_map();   //随机初始化生命游戏数组
    puts("*********************************\n*********************************");
	puts("\n      生    命    游    戏\n\n       欢迎来到生命游戏       \n"
         " 在这里您将体会到元胞自动机的乐趣\n\n          准备好了吗");
	puts("\n*********************************\n*********************************");
	help_display(); 
}


void updatewithinput()
{
    char buf[1024];
    while(1)
    {
        printf("请输入相应指令:");
        getInput(buf);
        long hashCode = hash(buf);
        system("cls");
        switch (hashCode)
        {
            case HELP:     
            help_display();
            break;

            case PRINT:    
            print_map();
            break;

            case RANDOM: 
            free_array();    
            random_map();
            print_map();
            break;
  
            case CATALOG:     
            catalog_display();
            break;

            case DESIGN:    
            design_map();
            break;

            case QUIT:     
            printf("当前未处于地图设计模式，指令无效，请检查输入！\n");
            break;

            case LOAD:     
            load_map();
            break;

            case SAVE:     
            save_map();
            break;

            case GENERATE:     
            step_map();
            break;

            case RUN:    
            auto_game();
            break;

            case EXIT:     
            printf("当前未处于自动运行游戏模式，指令无效，请检查输入！\n");
            break;

            case END:   
            free_array();
            return;

            default:
            printf("错误！请检查输入！\n");
            break;
        }
    }
}


long hash(char *a)
{
    long result = 0;
    for(int index = 0; index < strlen(a); index++)
    {
        result +=  a[index] * pow(128, index);
    }
    return result;
}


void getInput(char *buf)
{
    fgets(buf, 1024, stdin);
    buf[strlen(buf) - 1] = '\0';
}


void help_display()
{
    printf("\t[\\h]\t打印命令提示\n"
           "\t[\\p]\t打印当前地图\n"
           "\t[\\m]\t生成随机地图\n"
           "\t[\\c]\t打印文件目录\n"
           "\t[\\d]\t进入地图设计模式\n"
           "\t[\\q]\t退出地图设计模式\n"
           "\t[\\l <filename>]\t导入地图\n"
           "\t[\\s <filename>]\t保存地图\n"
           "\t[\\g]\t生成下一代生命\n"
           "\t[\\r]\t开始生命游戏\n"
           "\t[\\e]\t结束生命游戏\n"
           "\t[end]\t退出游戏\n");
}


void print_map()
{
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            if (game_array[i][j] == 1)
                printf("■");
            else
                printf("□");
            if (j == y - 1)
                printf("\n");
        }
}


void random_map()
{
    srand((unsigned)time(NULL));
    x = rand() % 100 + 1;
    y = rand() % 70 + 1;
    malloc_array();
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            game_array[i][j] = rand() % 2;
        }
}


void catalog_display()
{
    const char *path = {"./map/*.txt"};   //利用相对路径（map文件夹）
    long handle;    
    struct _finddata_t files;    
    handle = _findfirst(path, &files);  //第一次查找
    if(handle == -1)
        printf("无文件！\n");
    else
        printf("%s\n", files.name);
    while(!_findnext(handle, &files))   //循环查找其它地图文件
    {   
        printf("%s\n", files.name);
    }
    _findclose(handle);
}


void design_map()
{
    int temx, temy;
    int count, i, j;
    char buff[100];

    printf("已进入地图设计模式(输入\\q以退出)\n");
    printf("请输入细胞图行列数(eg：6 8)(注：行数要求1~100，列数要求1~70):\n");
    while(1)
    {
        count = scanf("%d%d", &temx, &temy);
        if (count == 0)         //正确输入参数为0时，检验是否执行退出,并对错误输入进行回应
        {
            fgets(buff, 3, stdin);
            if(strcmp(buff, "\\q") == 0 && getchar() == '\n')
            {
                printf("已退出地图设计模式\n");
                return;
            }
            else
            {
                while(getchar() != '\n')
                continue;
                printf("错误！请重新输入:\n");
                continue;
            }
        }
        else if (count == 2 && temx <= 100 && temx > 0 && temy <= 70 && temy > 0 && getchar() == '\n')
            break;                                  //确定正确输入情况的条件，并对错误输入进行回应
        else 
        {
            while(getchar() != '\n')
                continue;
            printf("错误！请重新输入:\n");
            continue;
        }    
    }

    free_array();
    x = temx;
    y = temy;
    malloc_array();
    for (i = 0; i < x; i++)
        for (j = 0; j < y; j++)
            game_array[i][j] = 0;               //分配内存并初始化数组
    printf("请输入存活细胞坐标(eg:0 0)(两坐标均为非负数且分别小于行列数):\n");
    while(1)
    {
        count = scanf("%d%d", &i, &j);
        if (count == 0) 
        {
            fgets(buff, 3, stdin);
            if (strcmp(buff, "\\q") == 0 && getchar() == '\n')
            {   
                system("cls");
                printf("地图已更新，可选择保存地图(\\s <filename>)\n");
                return;
            }
            else
            {
                printf("错误！请重新输入:\n");
                while (getchar() != '\n')
                    continue;
            }
        }
        else if (count == 2 && i < x && i >= 0 && j < y && j >= 0 && getchar() == '\n')
        {
            game_array[i][j] = 1;
            continue;
        }
        else    //对错误输入进行回应
        {
            printf("错误！请重新输入:\n");
            while(getchar() != '\n')
                continue;
        }
    }
}


void load_map()
{
    int i;
    char fname[200] = {"./map/"};   //保证相对路径
    char name[50];                  //输入文件名
    char *pname;                    //保证文件类型为txt文件
    FILE *fp;

    printf("请输入导入文件的文件名：\n");
    for (i = 0; i < 50; i++)
    {
        name[i] = (char)getchar();
        if (name[i] == '\n')
            break;
        if (i == 49)            //防止数组溢出
        {
            printf("文件名称输入有误或文件名过长，请尝试重新导入！\n");
            while (getchar() != '\n')
                continue;
            return;
        }
    }
    name[i] = '\0';
    strcat(fname, name);            //拼接路径与文件名

    pname = strrchr(fname, '.');    //进行文件类型判断
    if (pname == NULL || strcmpi(pname, ".txt") != 0)
    {
        printf("文件名称输入错误或文件格式错误，请尝试重新导入！\n");
        return;
    }

    if ((fp = fopen(fname, "r")) != NULL)
    {
        free_array();
        fscanf(fp, "%d%d", &x, &y);
        malloc_array();
        for (int i = 0; i < x; i++)
            for (int j = 0; j < y; j++)
                fscanf(fp, "%d", &game_array[i][j]);
        print_map();
    }
    else
    {
        printf("文件打开失败，请尝试重新导入！\n");
        return;
    }
}


void save_map()
{
    FILE *fp;
    int j;
    char fname[200] = {"./map/"};   
    char name[50];                  
    char *pname;

    printf("请输入保存文件的文件名：\n");
    for (j = 0; j < 50; j++)
    {
        name[j] = (char)getchar();
        if (name[j] == '\n')
            break;
        if (j == 49)
        {
            printf("名称有误，请重新保存\n");
            while (getchar() != '\n')
                continue;
            return;
        }
    }
    name[j] = '\0';
    strcat(fname, name);

    pname = strrchr(fname, '.');
    if (pname == NULL || strcmpi(pname, ".txt") != 0)
    {
        printf("文件名称输入错误或文件格式错误，请尝试重新导入！\n");
        return;
    }

    fp = fopen(fname, "w");
    fprintf(fp, "%d %d\n", x, y);
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            fprintf(fp, "%2d", game_array[i][j]);
            if (j == y - 1)
                fprintf(fp, "\n");
        }
    fclose(fp);
    printf("已保存地图。\n");    
}


int alive_count(int p, int q)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if (p + i >= 0 && p + i < x && q + j >= 0 && q + j < y)
                count += game_array[p + i][q + j];        
    return (count - game_array[p][q]);
}


void step_map()
{
    int **alive_array;              //声明二级指针，申请存储空间，记录每个细胞的周围存活个数
    alive_array = (int **)malloc(sizeof(int*) * x);
    for (int i = 0; i < x; i++)
        alive_array[i] = (int *)malloc(sizeof(int) * y);
    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
            alive_array[i][j] =  alive_count(i, j);

    for (int i = 0; i < x; i++)
        for (int j = 0; j < y; j++)
        {
            if (game_array[i][j] == 1)
            {
                if (alive_array[i][j] < 2 || alive_array[i][j] > 3)
                    game_array[i][j] = 0;
            }
            else if(game_array[i][j] == 0)
            {
                if (alive_array[i][j] == 3)
                    game_array[i][j] = 1;
            }    
        }
    print_map();
    for (int i = 0; i < x; i++)
        free(alive_array[i]);
    free(alive_array);    
}


void auto_game()
{
    char ch1, ch2;
    print_map();
    while(1)
    {
        Sleep(1000);
        system("cls");
        step_map();
        
        while(1)
        {   
            if (kbhit())        //判断键盘输入
            {
                ch1 = (char)getch();
                if (ch1 == '\r')    //若回车则暂停
                {
                    printf("输入回车继续生命游戏，输入\"\\e\"结束生命游戏，其他字符无效\n");   
                    ch2 = (char)getch();
                    while(1)
                    {
                        if (ch2 == '\r')    //再次回车继续游戏
                            break;
                        else if ((ch2 == '\\') & ((ch2 = (char)getch()) == 'e'))    //"\e"退出自动模式
                            {
                                printf("退出自动模式，结束生命游戏\n");
                                return;
                            }
                    }
                }
            }
            else break;     //其他输入无效
        } 
        
    }   
}


void malloc_array()
{
    game_array = (int **)malloc(sizeof(int*) * x);
    for (int i = 0; i < x; i++)
        game_array[i] = (int *)malloc(sizeof(int) * y); 
}


void free_array()
{
    for (int i = 0; i < x; i++)
        free(game_array[i]);
    free(game_array);
}
        

