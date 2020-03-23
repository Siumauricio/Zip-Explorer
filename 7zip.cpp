
#include <QCoreApplication>
#include <iostream>
#include <quazip.h>
#include <QStringList>
#include <QtDebug>
#include <JlCompress.h>
#include <QByteArrayList>

QByteArray recibirData(QString);
bool isDir(QString);

using namespace std;
int main()
{
       QuaZip archive("//home//siumauricio//Escritorio//7zip.zip");
       QString dir="16Bits/24Bits2/Test1/";
        QStringList splitter=dir.split("/");
        QString directorio=splitter[splitter.size()-2];
        int cantidad=splitter.size()-1;
         archive.open(QuaZip::mdUnzip);
        QStringList lista=archive.getFileNameList();
        //QString nombre="16Bits/24Bits2/Test1/24Bits2/";

        const QString substring(directorio);

      for (int i = 0; i < lista.size(); ++i) {
            int existe=lista[i].split("/").size();
            if(cantidad<existe){
                if(lista[i].split("/")[splitter.size()-2]==directorio){
                     QByteArray data = recibirData(lista[i]);
                     if(isDir(lista[i])){
                          QDir().mkdir("//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring)));
                          //  qDebug()<<"//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring));
                     }else{
                         QFile outfile("//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring)));
                        // qDebug()<<"//home//siumauricio//Documentos//"+lista[i].mid(lista[i].indexOf(substring));
                          outfile.open(QIODevice::WriteOnly);
                         outfile.write(data);
                     }

                }
            }
        }

}






QByteArray recibirData(QString path){
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

bool isDir(QString nombre)
{
     return nombre.toStdString()[nombre.length()-1]=='/';
}
