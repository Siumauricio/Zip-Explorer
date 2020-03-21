#include "mainwindow.h"
#include <QApplication>
#include <JlCompress.h>
#include <quazip.h>
#include <QtCore>
#include <zlib.h>
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    /*
    QuaZip archive("//home//siumauricio//Escritorio//16Bits4.zip");
    QString dir="16Bits/24Bits2";
    //qDebug()<<splitFileName[splitFileName.size()-2];
    QString dst;
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
                     if(str3[str3.length()-1]!='/'){
                         QStringList splitFileName = zFile.getFileName().split("/");
                         QString nombreArchivo=splitFileName[splitFileName.length()-1];
                         string text=dst.toStdString().substr(0,dst.length()-2);
                         QString path=dst;
                         dst+=nombreArchivo;
                         QFile outfile(dst);
                         outfile.open(QIODevice::WriteOnly);
                         outfile.write(ba);
                         dst=path;
                         qDebug()<<dst;
                     }else{
                         bool flag=false;
                         bool flag2=true;
                         QString path;
                        QStringList splitFileName = zFile.getFileName().split("/");
                         QStringList splitDir = dir.split("/");
                       //qDebug()<<splitFileName[splitFileName.size()-2];
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
                                     path+="//";
                                }
                            }
                        }
                     dst="//home//siumauricio//Documentos//"+path;

                       // qDebug()<<zFile.getFileName();
                     QDir().mkdir("//home//siumauricio//Documentos//"+path);
                     }
                 }
            }*/


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
