#ifndef RWFILE_H
#define RWFILE_H

#include <stdio.h>
#include "filestruct.h"
#include <QTime>
#include <QDateTime>


class RWFile
{
public:
    RWFile(FILE *infile);
    quint32 createFolder(struct pointStruct intemp, quint32 inode=0, QStringList *path=NULL);
    bool deleteFolder(QString filename,quint32 inode=0,QStringList *path=NULL);

    bool createFile(pointStruct intemp, quint32 inode, QStringList *path);
    bool deleteFile(QString filename,quint32 inode,QStringList *path);

    quint32 useBlock();//0 faile  other can use
    bool deleteBlock(quint32 blockId);

    quint32 readFile(quint32 id, QByteArray &filebuf);
    quint32 writeFile(quint32 id, QByteArray &buf);//type first 0 second 2

    bool changeState(quint32 id,quint32 state);

    quint32 readQuint32(){
        quint32 temp;
        fread(&temp,sizeof(quint32),1,file);
        return temp;
    }
    bool showMenu(quint32 inode, QStringList *path, QList<struct pointStruct> &menu, QList<quint32> &menuID);

    void writeQuint32(quint32 temp){
        fwrite(&temp,sizeof(quint32),1,file);
    }
private:
    FILE *file;
    bool deleteInfor(struct pointStruct fileInfor,struct pointStruct *buf);
};

#endif // RWFILE_H
