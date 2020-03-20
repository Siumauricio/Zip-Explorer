#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
namespace Ui {
class Dialog;
}
    QString getTexto(QString);
class Dialog : public QDialog
{
    Q_OBJECT

public:
     Ui::Dialog *ui;
     Dialog(QWidget *parent,QString&,QString&,QString&);
     Dialog(QString&);
    ~Dialog();
private slots:
     void on_textBrowser_historyChanged();

private:

};

#endif // DIALOG_H
