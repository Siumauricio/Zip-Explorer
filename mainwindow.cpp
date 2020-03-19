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
#include <quazipfile.h>

const QString s1 = "/Documentos/16Bits.zip/24Bits2/test.txt";
const QString s2 = "/Documentos/16Bits.zip/comandos";
const QString s3 = "/Documentos/16Bits.zip/32SA/comandos/pedro/jose.txt";
const QString s6 = "/Documentos/16Bits.zip/32SA/comandos/pedro/prueba.txt";
QList<QString>listap;
 QList<QTreeWidgetItem *>lista;

QString treeItemToFullPath(QTreeWidgetItem* treeItem);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QTreeWidget *treeWidget = ui->treeWidget;
    QStringList fileNames;
    fileNames << s1 <<s2<<s3<<s6;
    bool cerrar=true;
    int x=0;
    QTreeWidgetItem *topLevelItem = NULL;

    foreach (const QString &fileName, fileNames)
         {
          qDebug()<<"*************************************"<<endl;
             QStringList splitFileName = fileName.split("/");

             // add root folder as top level item if treeWidget doesn't already have it
             if (treeWidget->findItems(splitFileName[0], Qt::MatchFixedString).isEmpty())
             {
                 topLevelItem = new QTreeWidgetItem;
                 topLevelItem->setText(0, splitFileName[0]);
                 treeWidget->addTopLevelItem(topLevelItem);
                 qDebug()<< "Root: "<<splitFileName[0];
                // lista.push_back(topLevelItem);
                listap.push_back("ROOT");

             }

             QTreeWidgetItem *parentItem = topLevelItem;

             // iterate through non-root directories (file name comes after)
             for (int i = 1; i < splitFileName.size() - 1; ++i)
             {
                 // iterate through children of parentItem to see if this directory exists
                 bool thisDirectoryExists = false;
                 for (int j = 0; j < parentItem->childCount(); ++j)
                 {
                     if (splitFileName[i] == parentItem->child(j)->text(0))
                     {
                         thisDirectoryExists = true;
                        // lista.push_back(parentItem);
                         parentItem = parentItem->child(j);
                         if(cerrar){
                         listap.push_back(splitFileName[i]);
                             cerrar=false;
                         }

                         // listap.push_back(splitFileName[j]);

                         break;
                     }
                 }
                qDebug()<<endl;
                 if (!thisDirectoryExists)
                 {
                     parentItem = new QTreeWidgetItem(parentItem);
                     parentItem->setText(0, splitFileName[i]);
                    //  lista.push_back(parentItem);
                      parentItem->setIcon(0,QIcon("/home/siumauricio/Escritorio/ImagenZip/Dirs-Files/folder.png"));
                        qDebug()<<splitFileName[i];
                 }else{
                        qDebug()<<splitFileName[i];
                 }
                   // qDebug()<<splitFileName[i];

             }
             //Anade los archivosss
             QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
             childItem->setText(0, splitFileName.last());
             childItem->setIcon(0,QIcon("/home/siumauricio/Escritorio/ImagenZip/Dirs-Files/files.png"));
             // childItem->setText(1,"1829336625");
                   // lista.push_back(childItem);
                  listap.push_back(fileName);
                  listap.push_back(splitFileName.last());
                     qDebug()<<splitFileName.last();
                     qDebug()<<"*************************************"<<endl;
             }

               setCentralWidget(treeWidget);


             //  QString texto=treeItemToFullPath(lista[4]);
             //  qDebug()<<texto <<endl;

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


void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{

    QString url = dirModel->fileInfo(index).filePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void MainWindow::on_treeWidget_clicked(const QModelIndex &index)
{
    /*
    QModelIndex index2 = ui->treeWidget->currentIndex();
    // or
    QTreeWidgetItem *item = ui->treeWidget->currentItem();


  qDebug()<<endl;
      qDebug()<<listap[index2.row()];
    qDebug()<<endl;
    for (int i = 0; i < listap.size(); ++i) {
          qDebug()<<listap[i];
    }*/

    //qDebug()<<index.row()<<endl;
   //  qDebug()<<index.column()<<endl;
  //  QString texto=treeItemToFullPath(lista[index.row()]);
  //  qDebug()<<texto <<endl;
/*
    for( int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i )
    {

       //QTreeWidgetItem *item = ui->treeWidget->topLevelItem( index.column() );



    }*/



  //  QString item2 = ui->treeWidget->currentItem()-> ;

    QString item = ui->treeWidget->currentItem()->text(index.column());
   // qDebug()<<item<<endl;


   // value = item->data();
//    QTreeWidgetItem *item_parent = item->parent();

/*
    QModelIndexList selectedIndexes = ui->treeWidget->selectionModel()->selectedRows(index.column());
 QStringList selectedTexts;
 foreach(const QModelIndex &idx, selectedIndexes)
 {
     selectedTexts << idx.data(Qt::DisplayRole).toString();
     qDebug()<<selectedTexts[0];
 }*/
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
   QTreeWidgetItem * prueba=item->parent();
   if(prueba!=nullptr){
        qDebug()<<treeItemToFullPath(item);
   }


}
