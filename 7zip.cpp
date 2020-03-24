
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
       archive.open(QuaZip::mdUnzip);
       QString dir="/16Bits/24Bits2/Test1";
       QStringList splitter=dir.split("/");
       qDebug()<<splitter;
       string parentstring = "/16Bits/24Bits2/Test1-";
       QStringList lis= QString::fromStdString(parentstring).split("/");
       string substring = lis[lis.size()-1].toStdString();
       size_t index= parentstring.find(substring)-1;
       cout<<substring<<endl;

       //qDebug()<<lis;

        QString directorio=splitter[splitter.size()-1];
        int cantidad=splitter.size()-1;

        QStringList lista=archive.getFileNameList();

      for (int i = 0; i < lista.size(); ++i) {
            int existe=lista[i].split("/").size();
            if(cantidad<existe){
                if(lista[i].split("/")[splitter.size()-2]==directorio){
                    string recortada=lista[i].toStdString().substr(index,lista[i].length());
                     if(isDir(lista[i])){
                         cout<<recortada<<endl;
                       QDir().mkdir("//home//siumauricio//Documentos//"+QString::fromStdString(recortada));
                     }else{
                       QByteArray data = recibirData(lista[i]);
                       QFile outfile("//home//siumauricio//Documentos//"+QString::fromStdString(recortada));
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
                      return ba;
              }
       }
}

bool isDir(QString nombre) {
     return nombre.toStdString()[nombre.length()-1]=='/';
}
