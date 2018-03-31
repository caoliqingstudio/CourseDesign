#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filestruct.h"
#include <QMessageBox>
#include <QList>
#include "rwfile.h"
#include "menu.h"

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    QList<struct userStruct> users;
    FILE *file;
    void init();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
