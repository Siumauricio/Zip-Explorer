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

   //  QuaZip zip("//home//siumauricio//Escritorio//comandos.zip");
     //zip.open(QuaZip::mdUnzip);

        // extracts all files from the archive and saves
        // them onto disk
        //
        // just a simple implementation for evaluation.
        // error handling has not been implemented yet!
     JlCompress asd;
   //  QString abc=asd.extractFiles("//home//siumauricio//Escritorio//comandos.zip","comandos","//home//siumauricio//Escritorio//tes.txt");


     // asd->extractFile(zip," ", "//home//siumauricio//Documentos");



/*
     if (zip.open(QuaZip::mdUnzip)) {


         for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
             // do something
             qDebug() << zip.getCurrentFileName();
             zip.getUnzFile();

         }
         if (zip.getZipErcomando para buscar archivo en linuxror() == UNZ_OK) {
             // ok, there was no error
         }
     }

*/

QuaZip archive("//home//siumauricio//Escritorio//16Bits.zip");
QFile outfile("//home//siumauricio//Escritorio//test23");
int x=0;

outfile.open(QIODevice::WriteOnly);
    archive.open(QuaZip::mdUnzip);
        for( bool f = archive.goToFirstFile(); f; f = archive.goToNextFile() )
        {
            // set source file in archive
            QString filePath = archive.getCurrentFileName();
            QuaZipFile zFile( archive.getZipName(), filePath );
            // open the source file
            zFile.open( QIODevice::ReadOnly );
            // create a bytes array and write the file data into it
            QByteArray ba = zFile.readAll();
            qDebug()<<ba.size()<<endl;
            // close the source file
            if(x==0){
               QDir().mkdir("MyFolder");
                outfile.write(ba);
                break;
            }
            zFile.close();
            // set destination file
            QFile dstFile( filePath );
            // open the destination file
            dstFile.open( QIODevice::WriteOnly | QIODevice::Text );
            // write the data from the bytes array into the destination file
            dstFile.write( ba.data() );
            //close the destination file
            dstFile.close();
            x++;
        }



    /*
    JlCompress *asd;
      QuaZip zip;
      QString List=asd->extractFiles()
      asd->extractDir("//home//siumauricio//Escritorio//24Bits2.zip", "//home//siumauricio//Documentos");
    */
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
