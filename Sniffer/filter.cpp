#include "filter.h"
#include "ui_filter.h"

Filter::Filter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Filter)
{
    ui->setupUi(this);
    init();
}

Filter::~Filter()
{
    closefile();
    delete ui;
}

void Filter::init(){
    model = new QStringListModel(this);
    //配置文件
    QFile file(FILE_NAME);
    if(!file.exists()){
        if(!file.open(QIODevice::WriteOnly)){
            exit(0);
        }
        index = 0;
        list.append(FILTER_0);
        list.append(FILTER_1);
        list.append(FILTER_2);
        list.append(FILTER_3);
        list.append(FILTER_4);
        list.append(FILTER_5);
        file.close();
        closefile();
    }
    openfile();
    flushList();
//    qDebug()<<" list = :\n "<<list;
}

char* Filter::getString(){
    strBA=(list.at(index)).toLatin1();
    return strBA.data();
}

void Filter::openfile(){
    list.clear();
    QFile file(FILE_NAME);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       qDebug()<<"Can't open the file!"<<endl;
    }
    QTextStream in(&file);
    QString line;
    line = in.readLine();
    //line.chop(1);
    index=line.toInt();
    while(!in.atEnd()){
        line = in.readLine();
        //line.chop(1);
        list.append(line);
    }
    file.close();
    ui->label->setText(QString::number(index));
}


void Filter::closefile(){
    QFile fileout(FILE_NAME);
    if(!fileout.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open failed";
    }
    QTextStream txtOutput(&fileout);
    txtOutput<<QString::number(index)<<"\n";
    for(int i = 0; i < list.count();i++){
        txtOutput << list.at(i) << "\n";
    }
    fileout.close();
}

//确认
void Filter::on_pushButton_3_clicked()
{
    index=newIndex;
    this->hide();
    closefile();
   emit  filterChanged(getString());
}

void Filter::on_listView_doubleClicked(const QModelIndex &index)
{
    newIndex = index.row();
    ui->label->setText(QString::number(newIndex));
}

void Filter::on_pushButton_2_clicked()
{
    setIndex = ui->listView->currentIndex().row();
    ui->lineEdit->setText(list.at(setIndex));
    list.removeAt(setIndex);
    flushList();
}

void Filter::flushList(){
    model->setStringList(list);
    //qDebug()<<list;
    ui->listView->setModel(model);
}

void Filter::on_pushButton_4_clicked()
{
    newIndex =index;
    this->hide();
}

void Filter::on_pushButton_clicked()
{
    list.append(ui->lineEdit->text());
    flushList();
    closefile();
}


void Filter::on_listView_clicked(const QModelIndex &index)
{
    newIndex = index.row();
    ui->label->setText(QString::number(newIndex));
}
