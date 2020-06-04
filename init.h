#ifndef INIT_H
#define INIT_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCKSIZE 1024 // 磁盘块大小，一般为512B的2^n次倍
#define BLOCKNUM 1000 // 磁盘块数量
#define SIZE BLOCKNUM*BLOCKSIZE // 磁盘空间大小
#define END 65535 // FAT中文件结束标志
#define FREE 0 // FAT中盘块空闲标志
#define ROOTBLOCKNUM 2 // 根目录初始所占盘块大小
#define MAXOPENFILE 10 // 同时打开最大文件数
#define MAX_TEXT_SIZE 10000 // 输入文本缓冲区大小

// 文件控制块
typedef struct FCB {
    char filename[8]; // 文件名
    char exname[3]; // 文件扩展名
    unsigned char attribute; // 0: 目录文件, 1: 数据文件
    unsigned short time; // 创建时间
    unsigned short date; // 创建日期
    unsigned short first; // 文件起始盘块号
    unsigned short length; // 文件长度（字节数）
    char free; // 0: 空 1: 已分配
} fcb;

/**
 * 记录每个文件所占据磁盘块的块号
 * 记录哪些块被分出去
 * 值为FREE表示空闲，值为END表示文件最后一个磁盘号
*/
typedef struct FAT {
    unsigned short id;
} fat;

/**
 * 用户打开文件表
*/
typedef struct USEROPEN {
    char filename[8]; // 文件名
    char exname[3]; // 扩展名
    unsigned char attribute; //属性 0:目录文件 1:数据文件
    unsigned short time; // 创建时间
    unsigned short date; // 创建日期
    unsigned short first; // 文件起始盘块号
    unsigned short length; // 文件长度 (字节数)
    char free;

    //以上是FCB的信息

    int dirno; // 父目录文件起始盘块号
    int diroff; // 该文件对应的 fcb 在父目录中的逻辑序号
    char dir[100]; // 全路径信息
    int count; // 读写指针位置
    char fcbstate; // 是否修改 1是 0否
    char topenfile; // 打开表 0: 空 否则表示占用
} useropen;

// 引导区
typedef struct BLOCK {
    char information[200]; // 描述信息 磁盘块大小 磁盘块数量
    unsigned short root; // 根目录起始盘块号
    unsigned char* startblock; // 数据区起始位置
} block0;

// 全局变量

unsigned char* myvhard; // 指向虚拟磁盘的起始地址
useropen openfilelist[MAXOPENFILE]; // 用户打开文件表数组
int currfd; // 当前目录的文件描述符fd
unsigned char* startp; // 数据区开始的位置
char* copytextbuf; // copy缓冲区

/* 系统主要命令及函数 */
void startsys(); // 启动文件系统
void my_format(); // 对虚拟磁盘进行初始化
void my_cd(char* dirname); // 更改当前目录
int do_read(int fd, int len, char* text);
int do_write(int fd, char* text, int len, char wstyle);
int my_read(int fd);
int my_write(int fd);

void exitsys();
void my_cd(char* dirname);
int my_open(char* filename);
int my_close(int fd);
void my_mkdir(char* dirname);
void my_rmdir(char* dirname);
int my_create(char* fullname);
void my_rm(char* filename); // 删除文件
void my_ls(); // 显示目录函数
void help(); // 帮助信息
void my_search(char* fullname); // 搜索

int get_free_openfilelist();
unsigned short int get_free_block();
void set_create_time(unsigned short* create_date, unsigned short* create_time);

void get_name(char* name, int size, char* rawname); //解决没有'\0'的问题

#endif
