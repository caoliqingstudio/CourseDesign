#include "edit.h"
#include "ui_edit.h"

edit::edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::edit)
{
    ui->setupUi(this);
}

edit::~edit()
{
    delete ui;
}

void edit::init(quint32 inUserState, pointStruct inPoint, quint32 inPointID,FILE*inFile){
    userState = inUserState;
    point=inPoint;
    pointID=inPointID;
    file =inFile;
    if(userState<point.state){
        ui->pushButton_2->setEnabled(false);
    }
    ui->label->setText(QString(point.name));
    ui->textEdit->clear();
    RWFile rwfile(file);
    QByteArray buf;
    rwfile.readFile(inPointID,buf);
    ui->textEdit->setText(QString(buf));
}

void edit::on_pushButton_clicked()
{
    this->hide();
    this->deleteLater();
}

void edit::on_pushButton_2_clicked()
{
    QString buf =ui->textEdit->toPlainText();
    QByteArray bufBA=buf.toLatin1();
    RWFile rwfile(file);
    rwfile.writeFile(pointID,bufBA);
    this->hide();
    this->deleteLater();
}
