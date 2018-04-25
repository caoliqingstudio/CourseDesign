#include "arpcheat.h"
#include "ui_arpcheat.h"

ArpCheat::ArpCheat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ArpCheat)
{
    ui->setupUi(this);
    thread =0;
}

ArpCheat::~ArpCheat()
{
    delete ui;
    if(thread&&!thread->isFinished()){
        thread->terminate();
        thread->quit();
    }
}

void ArpCheat::init(PcapUse *inpcap, int row){
    pcap=inpcap;
    this->row=row;
    ui->label_2->setText(pcap->indexDev->name);//网卡名
    pcap->chooseDev(row);
    ui->lineEdit->setText("222.20.105.254");
}




void ArpCheat::on_pushButton_clicked()
{
    QString wgipQs=ui->lineEdit->text();
    QString ipQs = ui->lineEdit_2->text();
    char wgip[32],ip[32];
    strcpy(wgip,wgipQs.toStdString().c_str());
    strcpy(ip,ipQs.toStdString().c_str());
    quint16 op;
    if(ui->checkBox->isCheckable()){
        op=ARP_OP_ALL;
    }else{
        op=ARP_OP_IP;
    }
    thread = new ThreadARP();
    thread->init(pcap->indexDev,0,wgip,op,ip);
    thread->start();

    Date * date = new Date();
    date->init(pcap,row);
    date->show();
    this->destroy();
}


void ArpCheat::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
        ui->lineEdit_2->setEnabled(false);
    }else{
        ui->lineEdit_2->setEnabled(true);
    }
}
