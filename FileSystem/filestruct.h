#ifndef FILESTRUCT_H
#define FILESTRUCT_H

#include <QInternal>
#include <stdio.h>
#include <QFile>
#include <QString>
#include <iostream>
#include <QList>

/**
0    user
1-400   free
401-1023  store point
**/

#define BLOCK_SIZE 1024        ///块的大小，一块为1024字节
#define FILESIZE (1024*1024*100)                  //模拟磁盘的文件的大小 100mb
#define STORE_FREE_BLOCK_NUM (400)            //存储空闲块的编号的区域的大小 单位BLOCK_SIZE 即块 1024字节
#define STORE_POINT_BLOCK_NUM 623   // 存储point节点的区域，即文件信息或文件夹信息的区域大小，单位同上。623是为了凑个1024数字没别的意思
#define FREE_MAX_NUMBER (1024*100-1024)  //存储文件内容的空闲块的数量 单位同上。-1024刚好是前面三块区域的大小
#define POINT_MAX_NUMBER ((1024*623-4)/sizeof(struct pointStruct))     //存储point节点最大数量，-4是四个字节存储数量。

#define DISK "size100M" //文件名
#define FREE_NODES ((BLOCK_SIZE)/sizeof(quint32)-2) //没用到，旧版本时的

#define MAX_QUINT32 0xffffffff  //状态标记point区域store point区域的一个节点是否占用，似乎没用到，，当时就考判断是否0
#define USER 1   //等同于sample 使用时候有点混乱 不知道用没用
#define GUEST 0		//游客 参考super sample
#define ISFILE 0   //pointStruct 用的 判断是否是文件
#define ISMENU 1	//如上，文件夹
#define SUPER 2	   //超级用户，用户等级判断 最高
#define SAMPLE 1	//普通用户
#define FIRST 0		//忘了干啥的了，貌似是为了在创建目录时候判断是否是创建根目录
#define SECOND 1    //参考上

// 用户的结构
struct userStruct{
    char userName[32];
    char passWord[32];
    quint32 state;//1 super  0 sample          
    quint32 id;             //未使用
    quint32 inode;  // blocks 指向根目录的位置标号 从1开始
};

//文件内容的 位于pointStruct中
struct fileStruct{
    quint32 blkNum;//number of blocks 1kb
    quint32 fileSize;//b
    quint32 type;
    quint32 incode[12];//id of blk    blocks
};

//文件或者文件夹   
struct pointStruct{
    quint32 pORc;//0 is nothis    0xffffffff is node
    char name[32];
    quint32 type;//0file   1memu
    quint32 state;   //权限创建的 如 2 1 0
    quint64 ctime;	//创建时间
    quint32 links;  //未使用 本来是硬链接数量
    union{
        struct{
            quint32 number;//number of file in the folder
            quint32 next;//block
            quint32 child[12]; //byte
        }folderInfor; //文件夹           
		//next 未使用 本意是结合union pointInBlock 将存储pointStruct的位置替换为存储child的块号
        struct fileStruct fileInfor; //文件                  
    }infor;
};

//未使用的 
union pointInBlock{
  struct pointStruct point;
  quint32 child[31];// 结构 【number】【  next】【  00000文件内容块号】
};

//未使用 旧版本的设置
struct freeStruct{
    quint32 number;
    quint32 next;//block
    quint32 nodes[FREE_NODES];//block
};

// 未使用 旧版本的设置
struct blockStruct{
    quint32 number;
    quint32 next;//block
    struct pointStruct points[(BLOCK_SIZE-8)/sizeof(struct pointStruct)];
};

#endif // FILESTRUCT_H
