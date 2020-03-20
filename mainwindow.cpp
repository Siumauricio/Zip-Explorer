#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QStyle>
#include <QWidgetItem>
#include <QFileSystemModel>
#include <quazip.h>
#include "dialog.h"
#include <zlib.h>
#include <QtGui>
#include <QDirModel>
#include <QFileDialog>
#include <QSplitter>
#include <QMessageBox>
#include <iostream>
#include "zip_explorer.h"
#include <zlib.h>
#include <JlCompress.h>
#include <quazipfile.h>

QString Local;
QString Central;
QString End;

QString actualZip;
QString nombreArchivo;
QString tipo;
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

    nombreArchivo=treeItemToFullPath(item);
   qDebug()<<treeItemToFullPath(item);

   if(item->child(column)==NULL){
       qDebug()<<"ES FILE";
       tipo="FILE";
   }else{
       tipo="DIR";
       qDebug()<<"ES DIRECTORIO";
   }

}

void MainWindow::on_actionopen_triggered()
{
    QString file_name= QFileDialog::getOpenFileName(this, tr("Open File"),"/home/siumauricio/Escritorio",tr("Zip (*.zip)"),0,QFileDialog::DontUseNativeDialog);
    actualZip=file_name;
    ZipExplorer z;
    z.HeaderFileLocal(file_name.toStdString());
    Local=QString::fromStdString(z.Local);
    Central=QString::fromStdString(z.Central);
    End=QString::fromStdString(z.End);

    QTreeWidget *treeWidget = ui->treeWidget;
    QStringList fileNames =z.getZipSystem();

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
      Local="";
      Central="";
      End="";

}

void MainWindow::on_actionextract_triggered()
{
   QString destino =  QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks|QFileDialog::DontUseNativeDialog);
   qDebug()<<destino;
   if(nombreArchivo==""){
       JlCompress:: extractDir (actualZip,destino);
       QMessageBox::information(this,"Enhorabuena!","Archivo Descomprimido Correctamente!");
   }else if(tipo=="DIR"){
        extraerSubDir(nombreArchivo,destino);
   }else if(tipo=="FILE"){
        ExtraerFile(nombreArchivo,destino);
   }

}

void MainWindow::on_actioninfo_triggered()
{
   std::string str = "Hello world";
   QString qstr = QString::fromStdString(str);
   Dialog * a=new Dialog(this,Local,Central,End);
   a->show();
}

void MainWindow::extraerSubDir(QString directorio,QString destino){
    QuaZip archive(actualZip);
    QString dir=QString::fromStdString(directorio.toStdString().substr(1,directorio.length()-1));
    QString dst;
  ///  qDebug()<<"***************************************************";

        archive.open(QuaZip::mdUnzip);
            for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() ) {

                QString filePath = archive.getCurrentFileName();
                QuaZipFile zFile( archive.getZipName(), filePath );

                zFile.open( QIODevice::ReadOnly );
                // create a bytes array and write the file data into it
                QByteArray ba = zFile.readAll();

                 std::string checkFolder = zFile.getFileName().toStdString().substr (0,dir.length());
                    cout<<checkFolder<<endl;
                 if(checkFolder==dir.toStdString()){
                     string str3=zFile.getFileName().toStdString();
                     if(str3[str3.length()-1]!='/'){
                         QStringList splitFileName = zFile.getFileName().split("/");
                         QString nombreArchivo=splitFileName[splitFileName.length()-1];
                         string text=dst.toStdString().substr(0,dst.length()-2);
                         dst+=nombreArchivo;
                         QFile outfile(dst);
                         outfile.open(QIODevice::WriteOnly);
                         outfile.write(ba);
                     }else{
                         bool flag=false;
                         bool flag2=true;
                         QString path;
                        QStringList splitFileName = zFile.getFileName().split("/");
                         QStringList splitDir = dir.split("/");

                     //  qDebug()<<splitFileName[splitFileName.size()-2];
                        for (int i = 0; i < splitFileName.size(); ++i) {
                            if(splitFileName[i]==splitDir[splitDir.size()-1]&&flag2){
                                flag2=false;
                                flag=true;
                            }
                            if(flag){
                                if(splitFileName[i]==""){
                                      path+=splitFileName[i];

                                }else{
                                   path+=splitFileName[i];
                                     path+="/";
                                }
                            }
                        }
                     dst=destino+"/"+path;
                   // qDebug()<<dst;
                     QDir().mkdir(destino+"/"+path);
                     }
                 }
            }
}

void MainWindow::ExtraerFile(QString file,QString dst){

    QuaZip archive(actualZip);
    QString dir= QString::fromStdString(file.toStdString().substr(1,file.length()-1));;


    qDebug()<<"***************************************************";

        archive.open(QuaZip::mdUnzip);
            for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() ) {

                QString filePath = archive.getCurrentFileName();
                QuaZipFile zFile( archive.getZipName(), filePath );

                zFile.open( QIODevice::ReadOnly );
                // create a bytes array and write the file data into it
                QByteArray ba = zFile.readAll();

                 std::string checkFolder = zFile.getFileName().toStdString().substr (0,dir.length());

                 if(checkFolder==dir.toStdString()){
                     string str3=zFile.getFileName().toStdString();
                     if(str3[str3.length()-1]!='/' &&dir==zFile.getFileName()){
                        //qDebug()<<zFile.getFileName();

                         QStringList splitFileName = zFile.getFileName().split("/");
                         QString nombreArchivo=splitFileName[splitFileName.length()-1];
                         QFile outfile(dst+"/"+nombreArchivo);
                         outfile.open(QIODevice::WriteOnly);
                         outfile.write(ba);
                         return;
                     }
                }
          }

}
