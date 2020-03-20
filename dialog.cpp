#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, QString & local,QString &Central,QString &Final) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->textBrowser->setText(local+Central+Final);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_textBrowser_historyChanged()
{

}
