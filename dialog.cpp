#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->btnStop->setEnabled(false);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btnStart_clicked()
{
    if  (m_server.listen(QHostAddress::Any,ui->spinBox->value())){
        qDebug() << this << "server started!";
        SetStarted(true);
    }else {
        qCritical() << m_server.errorString();
        SetStarted(false);
    }
}

void Dialog::on_btnStop_clicked()
{
    m_server.close();
    SetStarted(false);
}

void Dialog::SetStarted(bool started)
{
    if  (started){
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
    }else {
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
    }
}
