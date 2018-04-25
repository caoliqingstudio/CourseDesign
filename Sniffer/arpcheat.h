#ifndef ARPCHEAT_H
#define ARPCHEAT_H

#include <QMainWindow>
#include "state.h"
#include "pcapuse.h"
#include <stdlib.h>
#include "threadarp.h"
#include <stdio.h>
#include "date.h"

namespace Ui {
class ArpCheat;
}

class ArpCheat : public QMainWindow
{
    Q_OBJECT

public:
    explicit ArpCheat(QWidget *parent = 0);
    ~ArpCheat();
    void init(PcapUse *inpcap,int row);
private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked();

private:
    int row;
    ThreadARP *thread;

    PcapUse *pcap;
    Ui::ArpCheat *ui;
};

#endif // ARPCHEAT_H
