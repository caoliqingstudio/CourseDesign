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
    pacp=new PcapUse();

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setColumnCount(2);
    /*设置表格是否充满，即行末不留空*/
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(pacp->devs.count());
    //ui->tableWidget->resizeColumnsToContents();
    //ui->tableWidget->resizeRowsToContents();
    QStringList header;
    header<<"Name"<<"description";   //表头
    ui->tableWidget->setHorizontalHeaderLabels(header);
    cout<<pacp->devs.count()<<endl;
    for(int i=0;i<pacp->devs.count();i++){
        //ui->tableWidget->setItem(i,0,new QTableWidgetItem(pacp.devs.at(i).at(0)));
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(pacp->devs.at(i).at(1)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(pacp->devs.at(i).at(2)));
        //cout<<pacp.devs.at(i).at(0)<<pacp.devs.at(i).at(1)<<pacp.devs.at(i).at(2)<<endl;
    }
    ui->tableWidget->show();
}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    cout<<row<<endl;
    //pacp->openDev(row);
    Date * date = new Date();
    date->init(pacp,row);
    date->show();
    this->destroy();
}

void MainWindow::on_pushButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row>=0){
        ArpCheat *arp = new ArpCheat();
        arp->init(pacp,row);
        arp->show();
        this->destroy();
    }
}
