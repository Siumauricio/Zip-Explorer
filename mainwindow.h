#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QFileSystemModel>
#include"zip_explorer.h"
#include "dialog.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

void on_treeWidget_clicked(const QModelIndex &index);
void extraerSubDir(QString,QString);
void ExtraerFile(QString , QString);
void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

void on_actionopen_triggered();

void on_actionclose_triggered();

void on_actionextract_triggered();

void on_actioninfo_triggered();

private:

Ui::MainWindow *ui;
Ui::Dialog *ui2;
};

#endif // MAINWINDOW_H
