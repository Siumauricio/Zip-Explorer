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
bool isDeflate;
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
    isDeflate=z.isDeflate;
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
      tipo="";
      actualZip="";

}

void MainWindow::on_actionextract_triggered()
{
    if(!isDeflate){
         QMessageBox::information(this,"Error!","Error Formato No Soportado!");
    }else{
        QString destino =  QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks|QFileDialog::DontUseNativeDialog);
        qDebug()<<destino;
        if(nombreArchivo==""){
            JlCompress:: extractDir (actualZip,destino);
            //QMessageBox::information(this,"Enhorabuena!","Archivo Descomprimido Correctamente!");
        }else if(tipo=="DIR"){
             extraerSubDir(nombreArchivo,destino);
        }else if(tipo=="FILE"){
             ExtraerFile(nombreArchivo,destino);
        }
    }
}

void MainWindow::on_actioninfo_triggered()
{
   std::string str = "Hello world";
   QString qstr = QString::fromStdString(str);
   Dialog * a=new Dialog(this,Local,Central,End);
   a->show();
}

void MainWindow::extraerSubDir(QString dir2,QString destino){
    QuaZip archive(actualZip);
    archive.open(QuaZip::mdUnzip);

    dir2=QString::fromStdString(dir2.toStdString().substr(1,dir2.length()-1));
    dir2+="/";
    qDebug()<<dir2<< " "<<destino<<endl;
    QString dir=dir2;
     QStringList splitter=dir.split("/");
     QString directorio=splitter[splitter.size()-2];
     int cantidad=splitter.size()-1;

     QStringList lista=archive.getFileNameList();

    const QString substring(directorio);

       for (int i = 0; i < lista.size(); ++i) {
             int existe=lista[i].split("/").size();
             if(cantidad<existe){
                 if(lista[i].split("/")[splitter.size()-2]==directorio){
                      QByteArray data = recibirData(lista[i]);
                      if(isDir(lista[i])){
                           QDir().mkdir(destino+"/"+lista[i].mid(lista[i].indexOf(substring)));
                           //  qDebug()<<"//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring));
                      }else{
                          QFile outfile(destino+"/"+lista[i].mid(lista[i].indexOf(substring)));
                         // qDebug()<<"//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring));
                           outfile.open(QIODevice::WriteOnly);
                          outfile.write(data);
                      }
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


QByteArray MainWindow::recibirData(QString path){
       QuaZip archive("//home//siumauricio//Escritorio//7zip.zip");
      archive.open(QuaZip::mdUnzip);
           qDebug()<<"***************************************************";
              for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() ) {
                  QString filePath = archive.getCurrentFileName();
                  QuaZipFile zFile( archive.getZipName(), filePath );
                  zFile.open( QIODevice::ReadOnly );
                  // create a bytes array and write the file data into it
                  QByteArray ba = zFile.readAll();
                  if(path==zFile.getFileName()){
                     // qDebug()<<zFile.getFileName();
                      return ba;
                  }
              }
           }

bool MainWindow::isDir(QString nombre)
{
     return nombre.toStdString()[nombre.length()-1]=='/';
}



