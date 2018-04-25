#ifndef DATE_H
#define DATE_H

//应该是 data

#include <QMainWindow>
#include <QMessageBox>
#include "state.h"
#include "pcapuse.h"
#include "threadpcap.h"
#include "filter.h"
#include <QCloseEvent>
#include <QDateTime>

#include <stdio.h>
#define DATA_FILE_NAME "Receive_Data_"

namespace Ui {
class Date;
}

class Date : public QMainWindow
{
    Q_OBJECT

public:
    explicit Date(QWidget *parent = 0);
    ~Date();
    void init(PcapUse * inpcap,int row);
private slots:
    void on_pushButton_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

public slots:
    void filterChangeTable(char *str);
    void packetReceive(Infor *infor);
private:
    FILE * file;
    QList<Infor *> inforList;
    void closeEvent(QCloseEvent *event);    // 重写closeEvent的申明
    void start(int index);
    int table_i;
    Ui::Date *ui;
    PcapUse *pcap;
    Filter *filter;
    inline unsigned short ntohs(unsigned short in){
        char * x =(char *)&in;
        char y[2];
        y[1]=x[0];
        y[0]=x[1];
        return *(unsigned short *)y;
    }
    inline quint32 ntohl(quint32 in){
        char * x =(char *)&in;
        char y[4];
        y[0]=x[3];
        y[1]=x[2];
        y[2]=x[1];
        y[3]=x[0];
        return *(quint32 *)y;
    }

    void ipPackedge(QString &str,uchar *data);
    void arpPackedge(QString &str,uchar *data);
    void tcpPackedge(QString &str,uchar *data);
    void udpPackedge(QString &str,uchar *data);
    void icmpPackedge(QString &str,uchar *data);
    void showData(Infor *data);
    void httpPackedge(QString &str,uchar *data);
    uint datalength;
    void openDataFile();
    void addDataFile(Infor *infor);
    void closeDataFile();
    void countToZero();
    void flushCount();
    void dnsPackedge(QString &str,uchar *data);
    uint ipcount,tcpcount,arpcount,rarpcount,icmpcount,udpcount,othercount;
};

#endif // DATE_H
