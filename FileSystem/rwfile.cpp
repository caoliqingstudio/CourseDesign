#include "rwfile.h"

#define TOPOINT(x) (BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)+sizeof(quint32)+sizeof(struct pointStruct)*(x-1))
#define TOBLOCK(x) (BLOCK_SIZE*(x))

RWFile::RWFile(FILE *infile)
{
    file =infile;
}

//inode start from 1
quint32 RWFile::createFolder(struct pointStruct intemp, quint32 inode, QStringList *path){
    QDateTime time = QDateTime::currentDateTime();
    intemp.pORc=1;
    intemp.ctime=time.toTime_t();
    intemp.type=ISMENU;
    intemp.infor.folderInfor.number=0;
    intemp.infor.folderInfor.next=0;
    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
    quint32 number;
    number=readQuint32();
    if(number+1>POINT_MAX_NUMBER){
        std::cout<<"rwfile createfolder fail"<<std::endl;
        exit(1);
    }
    if(inode==0){//mulu

        //change
        struct pointStruct buf[POINT_MAX_NUMBER];
        fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
        for(int i=0;i<POINT_MAX_NUMBER;i++){
            if(!buf[i].pORc){
                buf[i]=intemp;
                fseek(file,TOPOINT(1),SEEK_SET);
                fwrite(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
                fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
                number++;
                fwrite(&number,sizeof(quint32),1,file);
                return i+1;
            }
        }
        return 0;
        //end change
/*        QList<struct pointStruct> pSs;
        for(int i=0;i<POINT_MAX_NUMBER;i++){
            struct pointStruct tempp;
            fread(&tempp,sizeof(struct pointStruct),1,file);
            if(tempp.pORc||(!tempp.links))pSs.append(tempp);
        }
        pSs.append(temp);
        number++;
        fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
        writeQuint32(number);
        for(int i=0;i<pSs.count();i++){
            fwrite(&pSs.at(i),sizeof(struct pointStruct),1,file);
        }
        if(pSs.count()<POINT_MAX_NUMBER){
            struct pointStruct nullin;
            memset(&nullin,0,sizeof(struct pointStruct));
            int i=pSs.count();
            while(i<POINT_path->at(i))MAX_NUMBER){
                fwrite(&nullin,sizeof(struct pointStruct),1,file);
            }
        }
        return pSs.count();*/
    }else{
        struct pointStruct buf[POINT_MAX_NUMBER],*temp;
        fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
        temp=&buf[inode-1];
        //mei you kao lv chao guo 12 de qingkuang*///////////////////////////////////////
        for(int i=1;i<path->count();i++){
            int state=0;
            for(int j=0;j<temp->infor.folderInfor.number;j++){
                if(!(path->at(i)).compare(buf[temp->infor.folderInfor.child[j]-1].name)){
                    state=1;
                    temp=&buf[temp->infor.folderInfor.child[j]-1];
                    break;
                }
            }
            if(!state){
                std::cout<<"rwfile cfolder"<<std::endl;
                exit(1);
            }
        }
        int freeBlock=0;
        for(int i=0;i<POINT_MAX_NUMBER;i++){
            if(!buf[i].pORc){
                freeBlock=i+1;
                break;
            }
        }
        if(freeBlock==0){
            std::cout<<"rwfile cfolder 2"<<std::endl;
            return 0;
        }
        temp->infor.folderInfor.child[(temp->infor.folderInfor.number)++]=freeBlock;
        buf[freeBlock-1]=intemp;
        fseek(file,TOPOINT(1),SEEK_SET);
        fwrite(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
        fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
        number++;
        fwrite(&number,sizeof(quint32),1,file);
        return freeBlock;
    }
    return 0;
}

bool RWFile::deleteFolder(QString filename, quint32 inode, QStringList *path){
    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
    quint32 number;
    number=readQuint32();
    if(number+1>POINT_MAX_NUMBER){
        std::cout<<"rwfile createfolder fail"<<std::endl;
        exit(1);
    }
    struct pointStruct buf[POINT_MAX_NUMBER],*temp;
    fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    temp=&buf[inode-1];
    //mei you kao lv chao guo 12 de qingkuang*///////////////////////////////////////
    for(int i=1;i<path->count();i++){
        int state=0;
        for(int j=0;j<temp->infor.folderInfor.number;j++){
            if(!(path->at(i)).compare(buf[temp->infor.folderInfor.child[j]-1].name)){
                state=1;
                temp=&buf[temp->infor.folderInfor.child[j]-1];
                break;
            }
        }
        if(!state){
            std::cout<<"rwfile cfolder"<<std::endl;
            exit(1);
        }
    }
    int j=0;
    for(;j<temp->infor.folderInfor.number;j++){
        if(!(filename.compare(buf[temp->infor.folderInfor.child[j]-1].name))){
            buf[temp->infor.folderInfor.child[j]-1].pORc=0;
            break;
        }
    }
    if(j==temp->infor.folderInfor.number){
        std::cout<<"rwfile df no this file"<<std::endl;
        return false;
    }
    QList<quint32> ids;
    for(int i=0,k=0;i<temp->infor.folderInfor.number;i++){
        if(i!=j){
            ids.append(k++);
        }
    }
    temp->infor.folderInfor.number--;
    for(int i=0;i<ids.count();i++){
        temp->infor.folderInfor.child[i]=ids.at(i);
    }

    fseek(file,TOPOINT(1),SEEK_SET);
    fwrite(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
    number--;
    fwrite(&number,sizeof(quint32),1,file);
    return true;
}

bool RWFile::createFile(struct pointStruct intemp, quint32 inode, QStringList *path){
    QDateTime time = QDateTime::currentDateTime();
    intemp.ctime=time.toTime_t();
    intemp.type=ISFILE;
    intemp.infor.fileInfor.blkNum=0;
    intemp.pORc=1;
    intemp.infor.fileInfor.fileSize=0;
    intemp.infor.fileInfor.type=0;
    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
    quint32 number;
    number=readQuint32();
    if(number+1>POINT_MAX_NUMBER){
        std::cout<<"rwfile createfolder fail"<<std::endl;
        exit(1);
    }
    struct pointStruct buf[POINT_MAX_NUMBER],*temp;
    fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    temp=&buf[inode-1];
    //mei you kao lv chao guo 12 de qingkuang*///////////////////////////////////////
    for(int i=1;i<path->count();i++){
        int state=0;
        for(int j=0;j<temp->infor.folderInfor.number;j++){
            if(!(path->at(i)).compare(buf[temp->infor.folderInfor.child[j]-1].name)){
                state=1;
                temp=&buf[temp->infor.folderInfor.child[j]-1];
                break;
            }
        }
        if(!state){
            std::cout<<"rwfile cfolder"<<std::endl;
            exit(1);
        }
    }
    int freeBlock=0;
    for(int i=0;i<POINT_MAX_NUMBER;i++){
        if(!buf[i].pORc){
            freeBlock=i+1;
            break;
        }
    }
    if(freeBlock==0){
        std::cout<<"rwfile cfolder 2"<<std::endl;
        return 0;
    }
    temp->infor.folderInfor.child[(temp->infor.folderInfor.number)++]=freeBlock;
    buf[freeBlock-1]=intemp;
    fseek(file,TOPOINT(1),SEEK_SET);
    fwrite(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
    number++;
    fwrite(&number,sizeof(quint32),1,file);
    return true;
}

bool RWFile::deleteFile(QString filename,quint32 inode,QStringList *path){
    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
    quint32 number;
    number=readQuint32();
    if(number+1>POINT_MAX_NUMBER){
        std::cout<<"rwfile createfolder fail"<<std::endl;
        exit(1);
    }
    struct pointStruct buf[POINT_MAX_NUMBER],*temp;
    fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    temp=&buf[inode-1];
    //mei you kao lv chao guo 12 de qingkuang*///////////////////////////////////////
    for(int i=1;i<path->count();i++){
        int state=0;
        for(int j=0;j<temp->infor.folderInfor.number;j++){
            if(!(path->at(i)).compare(buf[temp->infor.folderInfor.child[j]-1].name)){
                state=1;
                temp=&buf[temp->infor.folderInfor.child[j]-1];
                break;
            }
        }
        if(!state){
            std::cout<<"rwfile cfolder"<<std::endl;
            exit(1);
        }
    }
    int j=0;
    for(;j<temp->infor.folderInfor.number;j++){
        if(!(filename.compare(buf[temp->infor.folderInfor.child[j]-1].name))){
            buf[temp->infor.folderInfor.child[j]-1].pORc=0;
            break;
        }
    }
    deleteInfor(buf[temp->infor.folderInfor.child[j]-1],buf);
    if(j==temp->infor.folderInfor.number){
        std::cout<<"rwfile df no this file"<<std::endl;
        return false;
    }
    QList<quint32> ids;
    for(int i=0,k=0;i<temp->infor.folderInfor.number;i++){
        if(i!=j){
            ids.append(temp->infor.folderInfor.child[k++]);
        }
    }
    temp->infor.folderInfor.number--;
    for(int i=0;i<ids.count();i++){
        temp->infor.folderInfor.child[i]=ids.at(i);
    }

    fseek(file,TOPOINT(1),SEEK_SET);
    fwrite(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
    number--;
    fwrite(&number,sizeof(quint32),1,file);
    return true;
}

quint32 RWFile::useBlock(){
    fseek(file,BLOCK_SIZE,SEEK_SET);
    quint32 number=0;
    fread(&number,sizeof(quint32),1,file);
    if(!number){
        return 0;
    }
    fseek(file,BLOCK_SIZE+(number)*sizeof(quint32),SEEK_SET);
    quint32 result=0;
    fread(&result,sizeof(quint32),1,file);
    fseek(file,BLOCK_SIZE,SEEK_SET);
    number--;
    fwrite(&number,sizeof(quint32),1,file);
    return result;
/*    struct freeStruct temp;
    fread(&temp,sizeof(struct freeStruct),1,file);
    if(temp.number==0){
        return 0;
    }
    while(temp.next){
        thisNode=temp.next;
        fseek(file,BLOCK_SIZE * temp.next,SEEK_SET);
        fread(&temp,sizeof(struct freeStruct),1,file);
    }
    fseek(file,BLOCK_SIZE *thisNode,SEEK_SET);
    temp.number--;*/
}

bool RWFile::deleteBlock(quint32 blockId){
    fseek(file,BLOCK_SIZE,SEEK_SET);
    quint32 number=0;
    fread(&number,sizeof(quint32),1,file);

    number++;

    fseek(file,BLOCK_SIZE+(number)*sizeof(quint32),SEEK_SET);
    fwrite(&blockId,sizeof(quint32),1,file);

    fseek(file,BLOCK_SIZE,SEEK_SET);
    fwrite(&number,sizeof(quint32),1,file);
    return true;
}

quint32 RWFile::readFile(quint32 id, QByteArray &filebuf){
//    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
//    quint32 number;
//    number=readQuint32();
//    if(number+1>POINT_MAX_NUMBER){
//        std::cout<<"rwfile createfolder fail"<<std::endl;
//        exit(1);
//    }
//    struct pointStruct buf[POINT_MAX_NUMBER],*temp;
//    fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);

    fseek(file,TOPOINT(id),SEEK_SET);
    struct pointStruct filePoint;
    fread(&filePoint,sizeof(struct pointStruct),1,file);
    for(quint32 i=0;i<filePoint.infor.fileInfor.blkNum;i++){
        char buf[BLOCK_SIZE];
        fseek(file,TOBLOCK(filePoint.infor.fileInfor.incode[i]),SEEK_SET);
        fread(buf,sizeof(char),BLOCK_SIZE,file);
        QByteArray x;
        x.setRawData(buf,BLOCK_SIZE);
        filebuf.append(x,BLOCK_SIZE);//you wen ti de
    }
    filebuf=filebuf.left(filePoint.infor.fileInfor.fileSize);
    return filePoint.infor.fileInfor.fileSize;
}

quint32 RWFile::writeFile(quint32 id, QByteArray &buf){
    fseek(file,TOPOINT(id),SEEK_SET);
    struct pointStruct filePoint;
    fread(&filePoint,sizeof(struct pointStruct),1,file);
    deleteInfor(filePoint,0);//wei liyong

    quint32 number = (buf.count()+BLOCK_SIZE-1)/BLOCK_SIZE;
    char *startbuf=buf.data();
    quint32 left=buf.count();
    for(int i=0;i<number&&i<12;i++){
        quint32 blockId=useBlock();
        filePoint.infor.fileInfor.incode[i]=blockId;
        fseek(file,TOBLOCK(blockId),SEEK_SET);
        QByteArray x;
        x.setRawData(startbuf,BLOCK_SIZE>left?left:BLOCK_SIZE);
        fwrite(x.data(),sizeof(char),BLOCK_SIZE>left?left:BLOCK_SIZE,file);
        left-=BLOCK_SIZE;
        startbuf+=BLOCK_SIZE;
    }
    filePoint.infor.fileInfor.fileSize=buf.count();
    filePoint.infor.fileInfor.blkNum=number;
    fseek(file,TOPOINT(id),SEEK_SET);
    fwrite(&filePoint,sizeof(struct pointStruct),1,file);
    return buf.count();
}


bool RWFile::deleteInfor(struct pointStruct fileInfor, pointStruct *buf){
    quint32 number = fileInfor.infor.fileInfor.blkNum;
    for(int i=0;i<number;i++){
        deleteBlock(fileInfor.infor.fileInfor.incode[i]);
    }
    if(number>12){
        //hai mei kaolv
    }
    return true;
}

bool RWFile::showMenu(quint32 inode,QStringList *path, QList<pointStruct> &menu,QList<quint32> &menuID){
    fseek(file,(BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM)),SEEK_SET);
    quint32 number;
    number=readQuint32();
    if(number+1>POINT_MAX_NUMBER){
        std::cout<<"rwfile createfolder fail"<<std::endl;
        exit(1);
    }
    struct pointStruct buf[POINT_MAX_NUMBER],*temp;
    fread(buf,sizeof(struct pointStruct),POINT_MAX_NUMBER,file);
    temp=&buf[inode-1];
    //mei you kao lv chao guo 12 de qingkuang*///////////////////////////////////////
    //int pathCount =path->count();
    for(int i=1;i<path->count();i++){
        int state=0;
        for(int j=0;j<temp->infor.folderInfor.number;j++){
            if(!(path->at(i)).compare(buf[temp->infor.folderInfor.child[j]-1].name)){
                state=1;
                temp=&buf[temp->infor.folderInfor.child[j]-1];
                break;
            }
        }
        if(!state){
            std::cout<<"rwfile cfolder"<<std::endl;
            exit(1);
        }
    }
    for(int i=0;i<temp->infor.folderInfor.number;i++){
        menu.append(buf[temp->infor.folderInfor.child[i]-1]);
        menuID.append(temp->infor.folderInfor.child[i]);
    }
    return true;
}

bool RWFile::changeState(quint32 id, quint32 state){
    fseek(file,TOPOINT(id),SEEK_SET);
    struct pointStruct temp;
    fread(&temp,sizeof(struct pointStruct),1,file);
    temp.state=state;
    fseek(file,TOPOINT(id),SEEK_SET);
    fwrite(&temp,sizeof(struct pointStruct),1,file);
    return true;
}
