#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init(){
    QFile isfile(DISK);
    if(!isfile.exists()){
        file = fopen(DISK,"wb");
        if(file){
            quint32 num=0;
            for(int i =0;i<FILESIZE/4;i++){
                fwrite(&num,sizeof(num),1,file);
            }
            fclose(file);

            file=fopen(DISK,"rb+");
            fseek(file,BLOCK_SIZE,SEEK_SET);
            //change start
            quint32 bl_num = FILESIZE/BLOCK_SIZE-1-STORE_FREE_BLOCK_NUM-STORE_POINT_BLOCK_NUM;
            quint32 startNum = 1+STORE_FREE_BLOCK_NUM+STORE_POINT_BLOCK_NUM;
            fwrite(&bl_num,sizeof(quint32),1,file);
            for(quint32 i=0;i<bl_num;i++,startNum++){
                fwrite(&startNum,sizeof(quint32),1,file);
            }
            fseek(file,BLOCK_SIZE*(1+STORE_FREE_BLOCK_NUM),SEEK_SET);
            num=0;
            fwrite(&num,sizeof(quint32),1,file);
            //change end
/*            quint32 bl_num=(FILESIZE-BLOCK_SIZE*2)/BLOCK_SIZE;
            quint32 nodeInB = FREE_NODES;
            quint32 bfNum = (bl_num+nodeInB-1) / nodeInB;
            struct freeStruct temp;
            temp.next=0;
            quint32 startI = 1+bfNum;
            for(quint32 i=0,j=0,nblocki=2;i<bfNum;i++,startI++){
                temp.nodes[j++]=startI;
                if(j>=nodeInB){
                    temp.number=j;
                    if(i+1>=bfNum){
                        nblocki=0;
                    }
                    temp.next=nblocki;
                    j=0;
                    fwrite(&temp,sizeof(struct freeStruct),1,file);
                    if(i+1<bfNum){
                        fseek(file,BLOCK_SIZE * (nblocki),SEEK_SET);
                        nblocki++;
                    }
                }
            }*/
        }else{
            QMessageBox::warning(this,"warning","Cant create disk file;");
            exit(1);
        }
    }else{
        file=fopen(DISK,"rb+");
        if(!file){
            QMessageBox::warning(this,"warning","Cant open disk file;");
            exit(1);
        }
        quint32 userNum=0;
        fread(&userNum,sizeof(userNum),1,file);
        for(int i=0;i<userNum;i++){
            struct userStruct theuser;
            fread(&theuser,sizeof(struct userStruct),1,file);
            users.append(theuser);
        }
    }
}


//login
void MainWindow::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    for(int i=0;i<users.count();i++){
        if((!name.compare(QString(users.at(i).userName))) && (!password.compare(QString(users.at(i).passWord)))){
            menu* themenu=new menu;
            themenu->init("user",file,users.at(i));
            themenu->show();
            this->hide();
            return;
        }
    }
    QMessageBox::warning(this,"login","false");
}

//register
void MainWindow::on_pushButton_2_clicked()
{
    QString name=ui->lineEdit_3->text();
    QString password = ui->lineEdit_4->text();
    for(int i=0;i<users.count();i++){
        if((!name.compare(QString(users.at(i).userName)))){
            QMessageBox::warning(this,"warning","Same username;");
            return;
        }
    }
    if(users.count()+1 <= (BLOCK_SIZE-sizeof(quint32))/sizeof(struct userStruct)){
        struct userStruct temp;
        strcpy(temp.userName,name.toStdString().c_str());
        strcpy(temp.passWord,password.toStdString().c_str());
        temp.inode=0;
        if(!ui->comboBox->currentText().compare("super")){
            temp.state=SUPER;
        }else{
            temp.state=SAMPLE;
        }
        RWFile rwfile(file);
        struct pointStruct point;
        strcpy(point.name,"root");
        point.state=temp.state;
        temp.inode=rwfile.createFolder(point);
        fseek(file,users.count()*sizeof(struct userStruct)+sizeof(quint32),SEEK_SET);
        users.append(temp);
        fwrite(&temp,sizeof(struct userStruct),1,file);
        fseek(file,0,SEEK_SET);
        quint32 usernum=users.count();
        fwrite(&usernum,sizeof(quint32),1,file);
        QMessageBox::warning(this,"register","OK");
    }else{
        QMessageBox::warning(this,"warning","over username;");
        return;
    }
}

//guest
void MainWindow::on_pushButton_3_clicked()
{
    QString name=ui->lineEdit->text();
    for(int i=0;i<users.count();i++){
        if((!name.compare(QString(users.at(i).userName)))){
            menu* themenu=new menu;
            themenu->init("guest",file,users.at(i));
            themenu->show();
            this->hide();
            return;
        }
    }
    QMessageBox::warning(this,"login","false");
}
