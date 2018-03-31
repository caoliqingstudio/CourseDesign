#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
}

menu::~menu()
{
    delete ui;
}

void menu::init(QString instate, FILE *infile, userStruct inuser){
    user=inuser;
    userState = instate;
    file = infile;
    currternState = instate.compare("guest")?inuser.state:GUEST;
    ui->label->setText(inuser.userName);
    ui->label_2->setText(instate);
    path.append("root");
    ui->label_4->setText("root");
    showMenu();
}

void menu::showMenu(){
    RWFile rwfile(file);
    points.clear();
    pointID.clear();
    rwfile.showMenu(user.inode,&path,points,pointID);
    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("name"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("type"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("time"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("pm"));

    ui->tableView->setModel(model);

    for(int i = 0; i < points.count(); i++)
    {
        model->setItem(i,0,new QStandardItem(points.at(i).name));
        model->setItem(i,1,new QStandardItem(QString(points.at(i).type==ISMENU?"folder":"file")));
        QDateTime current_date_time;
        QString current_date;
        current_date_time = QDateTime::fromTime_t(points.at(i).ctime);
        current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        model->setItem(i,2,new QStandardItem(current_date));
        model->setItem(i,3,new QStandardItem(QString(points.at(i).state==USER?"user":points.at(i).state==SUPER?"super":"guest")));
    }
}


//create forlder
void menu::on_pushButton_clicked()
{
    QString filename = ui->lineEdit->text();
    RWFile rwfile(file);
    struct pointStruct point;
    strcpy(point.name,filename.toStdString().c_str());
    point.state=userState.compare("guest")?user.state:GUEST;
    rwfile.createFolder(point,user.inode,&path);
    showMenu();
    //rwfile.
}

void menu::on_pushButton_3_clicked()
{
    QString filename = ui->lineEdit->text();
    RWFile rwfile(file);
    struct pointStruct point;
    strcpy(point.name,filename.toStdString().c_str());
    point.state=userState.compare("guest")?user.state:GUEST;
    rwfile.createFile(point,user.inode,&path);
    showMenu();
}

//back
void menu::on_pushButton_5_clicked()
{
    if(path.count()>1){
        RWFile rwfile(file);
        path.removeLast();
        showMenu();
        ui->label_4->setText(path.join('/'));
    }
}

//delete file
void menu::on_pushButton_4_clicked()
{
    int row= ui->tableView->currentIndex().row();

    QAbstractItemModel *model = ui->tableView->model ();

    QModelIndex index = model->index(row,0);//选中行第一列的内容
    QString name = index.data().toString();
    RWFile rwfile(file);
    rwfile.deleteFile(name,user.inode,&path);
    showMenu();
}

void menu::on_pushButton_2_clicked()
{
    int row= ui->tableView->currentIndex().row();

    QAbstractItemModel *model = ui->tableView->model ();

    QModelIndex index = model->index(row,0);//选中行第一列的内容
    QString name = index.data().toString();
    RWFile rwfile(file);
    rwfile.deleteFolder(name,user.inode,&path);
    showMenu();
}

void menu::on_pushButton_6_clicked()
{
    quint32 state = ui->comboBox->currentIndex()?0:1;
    if(state>currternState){
        QMessageBox::warning(this,"warning","No permission!");
        return;
    }
    if(state!=0){
        state=currternState;
    }
    RWFile rwfile(file);
     int row= ui->tableView->currentIndex().row();
    rwfile.changeState(pointID.at(row),state);
    showMenu();
}

// open folder or file
void menu::on_pushButton_7_clicked()
{

    int row= ui->tableView->currentIndex().row();
    if(points.at(row).type==ISMENU){
        path.append(points.at(row).name);
        showMenu();
        ui->label_4->setText(path.join('/'));
    }else{
        edit *text= new edit(0);
        text->init(currternState,points.at(row),pointID.at(row),file);
        text->show();
    }
}
