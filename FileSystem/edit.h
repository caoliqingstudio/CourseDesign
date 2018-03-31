#ifndef EDIT_H
#define EDIT_H

#include <QWidget>
#include <QMessageBox>
#include "filestruct.h"
#include "rwfile.h"

namespace Ui {
class edit;
}

class edit : public QWidget
{
    Q_OBJECT

public:
    explicit edit(QWidget *parent = 0);
    quint32 userState;
    FILE *file;
    quint32 pointID;
    struct pointStruct point;
    void init(quint32 inUserState,struct pointStruct inPoint,quint32 inPointID,FILE*inFile);
    ~edit();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::edit *ui;
};

#endif // EDIT_H
