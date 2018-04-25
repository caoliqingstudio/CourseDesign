#ifndef FILTER_H
#define FILTER_H

//过滤器

#include <QMainWindow>
#include <QFile>
#include <QStringListModel>
#include "state.h"

#define FILTER_0 "tcp or udp or icmp"
#define FILTER_1 "ip or arp"
#define FILTER_2 "ether proto \\arp"
#define FILTER_3 "port 80 or port 8080"
#define FILTER_4 "src net 192.168.0.0"
#define FILTER_5 "ether broadcast"


#define FILE_NAME "setting"

namespace Ui {
class Filter;
}

class Filter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = 0);
    ~Filter();
    char * getString();
signals:
    void filterChanged(char *c);

private slots:
    void on_pushButton_3_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::Filter *ui;
    void init();
    int index,newIndex,setIndex;
    void openfile();
    void closefile();
    void flushList();
    QByteArray strBA;
    QStringListModel *model;
    QStringList list;
};

#endif // FILTER_H
