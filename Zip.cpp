#include "zip_explorer.h"
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <QtDebug>

using namespace std;
ZipExplorer::ZipExplorer() {cantidadDirs=0;}

ZipExplorer::~ZipExplorer(){}

void ZipExplorer::HeaderFileLocal(string Filename) {
    isDeflate=false;
    hasData=false;
    ifstream Archivo;
    Archivo.open (Filename, ifstream::in | ifstream::binary);
    if(Archivo.fail()){
        return;
    }
    Archivo.seekg (0, ios_base::beg);
  while(CentralHeader.signature!=0){
        ImprimirLocalHeader(Archivo);
        if(isFile()){
            ImprimirDataDescriptor(Archivo);
        }
   }
  while (EndHeader.signature!=0){
    ImprimirCentralDirectory(Archivo,Filename);
  }
    Archivo.close();
}

void ZipExplorer::ImprimirLocalHeader(ifstream &Archivo) {
      int posAnterior=Archivo.tellg();
    Archivo.read ((char*)&LocalHeader, sizeof (LocalFileHeader));
    if(LocalHeader.signature!=67324752){
           Archivo.seekg(posAnterior,ios::beg);
           CentralHeader.signature=0;
           return;
       }
        int espacioR=LocalHeader.filenameLength;
        int espacioR2=LocalHeader.extraFieldLength;
        char *data=new char[espacioR];
        char *data2=new char[espacioR2];

        LocalInfo.Filename=new char[LocalHeader.filenameLength];
        Archivo.read(data,LocalHeader.filenameLength);
        memcpy(LocalInfo.Filename,data,LocalHeader.filenameLength);

        LocalInfo.extraField=new char[LocalHeader.extraFieldLength];
        Archivo.read(data2,LocalHeader.extraFieldLength);
        memcpy(LocalInfo.extraField,data2,LocalHeader.extraFieldLength);
        LocalInfo.Filename[LocalHeader.filenameLength]='\0';
        QString NameFile(LocalInfo.Filename);
        QString Name="/"+NameFile;

        if(Name[Name.length()-1]!='/'){
       //    qDebug()<< "Prueba "<<Name<<endl;
             zipSystem<<Name;
        }
        if(LocalHeader.compressionMethod==8&&!isDeflate){
            isDeflate=true;
        }

        string Filename(LocalInfo.Filename);
        string ExtraField(LocalInfo.extraField);
        Local+= "********LOCAL FILE HEADER********\n"  "Signature: " +to_string(LocalHeader.signature)+"\n"+ "versionToExtract: "+to_string(LocalHeader.versionToExtract)+"\n"+ "generalPurposeBitFlag: "+to_string(LocalHeader.generalPurposeBitFlag)+"\n"+ "compressionMethod: "+to_string(LocalHeader.compressionMethod)+"\n"+ "modificationTime: "+to_string(LocalHeader.modificationTime)+"\n"+ "modificationDate: "+to_string(LocalHeader.modificationDate)+"\n"+ "crc32: "+to_string(LocalHeader.crc32)+"\n"+ "compressedSize: "+to_string(LocalHeader.compressedSize)+"\n"+ "uncompressedSize: "+to_string(LocalHeader.uncompressedSize)+"\n"+ "filenameLength: "+to_string(LocalHeader.filenameLength)+"\n"+ "extraFieldLength: "+to_string(LocalHeader.extraFieldLength)+"\n"+ "Filename: "+Filename+"\n"+ "ExtraField: "+ExtraField+"\n************************************\n\n";
        cantidadDirs++;

}

uint8_t ZipExplorer::getDataDescriptor(ifstream &Archivo) {
    uint8_t i = 0;
    while (1) {
        Archivo.read((char *) &i, 1);
        if ((int) i == 80) {
            Archivo.read((char *) &i, 1);
            if ((int) i == 75) {
                Archivo.read((char *) &i, 1);
                if ((int) i == 3||(int) i==1||(int)i==7) {
                    Archivo.read((char *) &i, 1);
                    if ((int) i == 4||(int)i==2||(int)i==8) {
                        qDebug()<<i;
                        return i;
                    }
                }
            }
        }
    }

}

void ZipExplorer::ImprimirDataDescriptor(ifstream &Archivo) {

    uint8_t hasData=getDataDescriptor(Archivo);
    int x=Archivo.tellg();
    Archivo.seekg(x-4,ios::beg);

    if((int)hasData==8){
        Archivo.read((char*)&dataD, sizeof(DataDescriptor));
        cout<<dataD.signature<<" "<<dataD.crc32<<endl;
        Local+="**********DATA DESCRIPTOR**********\n" "signature: "+to_string(dataD.signature)+"\n"+"crc32: "+to_string(dataD.crc32)+"\n" +"compressedSize: "+to_string(dataD.compressedSize)+"\n" +"uncompressedSize: "+to_string(dataD.uncompressedSize)+"\n**************************************\n\n";
    }
}

void ZipExplorer::ImprimirCentralDirectory(ifstream &Archivo,string Filename) {
    int posAnterior=Archivo.tellg();

    Archivo.read ((char*)&CentralHeader, sizeof (CentralDirectory));
    if(CentralHeader.signature!=33639248){
        Archivo.close();
        Archivo.open (Filename, ifstream::in | ifstream::binary);
        Archivo.seekg(posAnterior,ios::beg);
        ImprimirEndDirectory(Archivo);
        EndHeader.signature=0;
        Archivo.close();
        return;
    }
    int espacioR=CentralHeader.filenameLength+CentralHeader.extraFieldLength+CentralHeader.fileComentlength;
    char *data=new char[espacioR];

    CentralInfo.Filename=new char[CentralHeader.filenameLength];
    Archivo.read(data,CentralHeader.filenameLength);
    memcpy(CentralInfo.Filename,data,CentralHeader.filenameLength);

    CentralInfo.extraField=new char[CentralHeader.extraFieldLength];
    Archivo.read(data,CentralHeader.extraFieldLength);
    memcpy(CentralInfo.extraField,data,CentralHeader.extraFieldLength);

    CentralInfo.FileComment=new char[CentralHeader.fileComentlength];
    Archivo.read(data,CentralHeader.fileComentlength);
    memcpy(CentralInfo.FileComment,data,CentralHeader.fileComentlength);
    string filename(CentralInfo.Filename);
    string ExtraField(CentralInfo.extraField);
      string FileComment(CentralInfo.FileComment);
       Central+="**********CENTRAL DIRECTORY**********\n" "signature: "+to_string(CentralHeader.signature)+"\n" +"versionMadeby: "+to_string(CentralHeader.versionMadeby)+"\n" +"generalPurposeBitFlag: "+to_string(CentralHeader.generalPurposeBitFlag)+"\n" +"compressionMethod: "+to_string(CentralHeader.compressionMethod)+"\n" +"modificationTime: "+to_string(CentralHeader.modificationTime)+"\n" +"modificationDate: "+to_string(CentralHeader.modificationDate)+"\n" +"crc32: "+to_string(CentralHeader.crc32)+"\n" +"compressedSize: "+to_string(CentralHeader.compressedSize)+"\n" +"uncompressedSize: "+to_string(CentralHeader.uncompressedSize)+"\n" +"filenameLength: "+to_string(CentralHeader.filenameLength)+ "\n"+"extraFieldLength: "+to_string(CentralHeader.extraFieldLength)+"\n"+"diskNumberStarts: "+to_string(CentralHeader.diskNumberStarts)+"\n"+"internalFileAttributes: "+to_string(CentralHeader.internalFileAttributes)+"\n"+"externalFileAttributes: "+to_string(CentralHeader.externalFileAttributes)+"\n"+"offsetLocalHeader: "+to_string(CentralHeader.offsetLocalHeader)+"\n"+"filename: "+filename+"\n"+"extraField: "+ExtraField+"\n"+"FileComment: "+FileComment+"\n**************************************\n\n";
}

bool ZipExplorer::isDirectory() {
    string filename=LocalInfo.Filename;
    return filename[filename.length()-1] == '/';
}

bool ZipExplorer::isFile() {
    string filename=LocalInfo.Filename;
    return filename[filename.length()-1] != '/';
}

void ZipExplorer::ImprimirEndDirectory(ifstream &Archivo) {
    Archivo.read ((char*)&EndHeader, sizeof (EndDirectory));
    int espacioR=EndHeader.commentLength;
    char *data=new char[espacioR];
    EndInfo.comment=new char[EndHeader.commentLength];
    Archivo.read(data,EndHeader.commentLength);
    memcpy(EndInfo.comment,data,EndHeader.commentLength);

    End+="**********END DIRECTORY**********\n" "Signature: "+to_string(EndHeader.signature)+"\n" +"numberDisk: "+to_string(EndHeader.numberDisk)+"\n" +"DiskWhereStarts: "+to_string(EndHeader.DiskWhereStarts)+"\n" +"numberCentralDirectory: "+to_string(EndHeader.numberCentralDirectory)+"\n" +"totalNumberDirectory: "+to_string(EndHeader.totalNumberDirectory)+"\n" +"sizeofDirectory: "+to_string(EndHeader.sizeofDirectory)+"\n" +"offsetDirectory: "+to_string(EndHeader.offsetDirectory)+"\n" +"commentLength: "+to_string(EndHeader.commentLength)+"\n" +"commentLength: "+to_string(EndHeader.commentLength)+"\n**************************************\n\n";
}


QStringList ZipExplorer::getZipSystem(){
   return this->zipSystem;
}
