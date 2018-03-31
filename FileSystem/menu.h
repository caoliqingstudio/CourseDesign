#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "filestruct.h"
#include <stdio.h>
#include "rwfile.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDataStream>
#include "edit.h"

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    QString userState;
    quint32 currternState;
    FILE *file;
    struct userStruct user;
    void init(QString instate,FILE *infile,struct userStruct inuser);
    ~menu();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    QList<struct pointStruct> points;
    QList<quint32> pointID;
    QStringList path;
    void showMenu();
    Ui::menu *ui;
};

#endif // MENU_H
