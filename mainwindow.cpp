#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QStyle>
#include <QWidgetItem>
#include <QFileSystemModel>
#include <quazip.h>
#include <zlib.h>
#include <QtGui>
#include <QDirModel>
#include <QFileDialog>
#include <QSplitter>
#include <QMessageBox>
#include "zip_explorer.h"
#include <quazipfile.h>

QString treeItemToFullPath(QTreeWidgetItem* treeItem);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setCentralWidget(ui->treeWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString treeItemToFullPath(QTreeWidgetItem* treeItem)
{
    QString fullPath= treeItem->text(0);

    while (treeItem->parent() != NULL)
    {
        fullPath= treeItem->parent()->text(0) + "/" + fullPath;
        treeItem = treeItem->parent();
    }
    return fullPath;
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{
    QString item = ui->treeWidget->currentItem()->text(index.column());
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
   QTreeWidgetItem * prueba=item->parent();
   if(prueba!=nullptr){
        qDebug()<<treeItemToFullPath(item);
   }

}

void MainWindow::on_actionopen_triggered()
{
    QString file_name=QFileDialog::getOpenFileName(this,"Open a File","//home//siumauricio//Escritorio");

    ZipExplorer z;
    z.HeaderFileLocal(file_name.toStdString());
    QTreeWidget *treeWidget = ui->treeWidget;

    QStringList fileNames= z.getZipSystem();
    QTreeWidgetItem *topLevelItem = NULL;

    foreach (const QString &fileName, fileNames) {
         QStringList splitFileName = fileName.split("/");
         if (treeWidget->findItems(splitFileName[0], Qt::MatchFixedString).isEmpty())
         {
             topLevelItem = new QTreeWidgetItem;
             topLevelItem->setText(0, splitFileName[0]);
             treeWidget->addTopLevelItem(topLevelItem);
             topLevelItem->setIcon(0,QIcon("/home/siumauricio/Escritorio/ImagenZip/Dirs-Files/folder.png"));
         }

         QTreeWidgetItem *parentItem = topLevelItem;
         for (int i = 1; i < splitFileName.size() - 1; ++i)
         {
             bool thisDirectoryExists = false;
             for (int j = 0; j < parentItem->childCount(); ++j)
             {
                 if (splitFileName[i] == parentItem->child(j)->text(0))
                 {
                     thisDirectoryExists = true;
                     parentItem = parentItem->child(j);
                     break;
                 }
             }

             if (!thisDirectoryExists)
             {
                 parentItem = new QTreeWidgetItem(parentItem);
                 parentItem->setText(0, splitFileName[i]);
                 parentItem->setIcon(0,QIcon("/home/siumauricio/Escritorio/ImagenZip/Dirs-Files/folder.png"));
             }
         }
         QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
         childItem->setText(0, splitFileName.last());
         childItem->setIcon(0,QIcon("/home/siumauricio/Escritorio/ImagenZip/Dirs-Files/files.png"));
       }

}

void MainWindow::on_actionclose_triggered()
{
     ui->treeWidget->clear();
}
