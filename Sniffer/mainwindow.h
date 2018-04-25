#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pcapuse.h"
#include "state.h"
#include "date.h"
#include "arpcheat.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    PcapUse* pacp;
    void init();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
