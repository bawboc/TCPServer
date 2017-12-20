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
    SetStarted(true);
}

void Dialog::on_btnStop_clicked()
{
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
