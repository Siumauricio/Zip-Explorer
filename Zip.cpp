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
    ifstream Archivo;
    Archivo.open (Filename, ifstream::in | ifstream::binary);
    if(Archivo.fail()){
        return;
    }
    Archivo.seekg (0, ios_base::beg);
    int pos=0;
    while(1){
        pos=Archivo.tellg();
        ImprimirLocalHeader(Archivo);
        if(isDirectory()){
            pos=Archivo.tellg();
            ImprimirLocalHeader(Archivo);
            if(strcmp(LocalInfo.Filename,"")==0){
                break;
            }
            if(isFile()){
                ImprimirDataDescriptor(Archivo);
            }
        }else{
            if(strcmp(LocalInfo.Filename,"")==0){
                break;
            }
            if(isFile()){
                ImprimirDataDescriptor(Archivo);
            }
        }
    }
    cantidadDirs--;
    Archivo.seekg(pos,ios::beg);
    while(cantidadDirs--){
        ImprimirCentralDirectory(Archivo);
    }
    ImprimirEndDirectory(Archivo);
    cantidadDirs=0;
    Archivo.close();
}

void ZipExplorer::ImprimirLocalHeader(ifstream &Archivo) {
    Archivo.read ((char*)&LocalHeader, sizeof (LocalFileHeader));
    if(LocalHeader.signature!=67324752){
        LocalInfo.Filename="";
        cantidadDirs++;
        return;
    }
    /*
        cout<<"**************LOCAL FILE HEADER**************"<<endl;
        cout<<"Signature: "<<LocalHeader.signature<<endl;
        cout<<"versionToExtract: "<<LocalHeader.versionToExtract<<endl;
        cout<<"generalPurposeBitFlag: "<<LocalHeader.generalPurposeBitFlag<<endl;
        cout<<"compressionMethod: "<<LocalHeader.compressionMethod<<endl;
        cout<<"modificationTime: "<<LocalHeader.modificationTime<<endl;
        cout<<"modificationDate: "<<LocalHeader.modificationDate<<endl;
        cout<<"crc32: "<<LocalHeader.crc32<<endl;
        cout<<"compressedSize: "<<LocalHeader.compressedSize<<endl;
        cout<<"uncompressedSize: "<<LocalHeader.uncompressedSize<<endl;
        cout<<"filenameLength: "<<LocalHeader.filenameLength<<endl;
        cout<<"extraFieldLength: "<<LocalHeader.extraFieldLength<<endl;*/
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


        /*
        cout<<"Filename: "<<LocalInfo.Filename<<endl;
        cout<<"ExtraField: "<<LocalInfo.extraField<<endl;
        cout<<"******************************************\n"<<endl;*/
        string Filename(LocalInfo.Filename);
        string ExtraField(LocalInfo.extraField);
        Local+= "********LOCAL FILE HEADER********\n"  "Signature: " +to_string(LocalHeader.signature)+"\n"+ "versionToExtract: "+to_string(LocalHeader.versionToExtract)+"\n"+ "generalPurposeBitFlag: "+to_string(LocalHeader.generalPurposeBitFlag)+"\n"+ "compressionMethod: "+to_string(LocalHeader.compressionMethod)+"\n"+ "modificationTime: "+to_string(LocalHeader.modificationTime)+"\n"+ "modificationDate: "+to_string(LocalHeader.modificationDate)+"\n"+ "crc32: "+to_string(LocalHeader.crc32)+"\n"+ "compressedSize: "+to_string(LocalHeader.compressedSize)+"\n"+ "uncompressedSize: "+to_string(LocalHeader.uncompressedSize)+"\n"+ "filenameLength: "+to_string(LocalHeader.filenameLength)+"\n"+ "extraFieldLength: "+to_string(LocalHeader.extraFieldLength)+"\n"+ "Filename: "+Filename+"\n"+ "ExtraField: "+ExtraField+"************************************\n\n";
       //cout<<test;
        cantidadDirs++;

}

void ZipExplorer::getDataDescriptor(ifstream &Archivo) {
    uint8_t i = 0;
    while (1){
        Archivo.read ((char*)&i, 1);
        if((int)i==80){
            Archivo.read((char*)&i,1);
            if((int)i==75){
                Archivo.read((char*)&i,1);
                if((int)i==7){
                    Archivo.read((char*)&i,1);
                    if((int)i==8){
                        return;
                    }
                }
            }
        }
    }
}

void ZipExplorer::ImprimirDataDescriptor(ifstream &Archivo) {
    getDataDescriptor(Archivo);
    Archivo.read((char*)&dataD,sizeof(DataDescriptor));/*
    cout<<"**************DATA DESCRIPTOR**************"<<endl;
    cout<<"crc32: "<<dataD.crc32<<endl;
    cout<<"compressedSize: "<<dataD.compressedSize<<endl;
    cout<<"uncompressedSize: "<<dataD.uncompressedSize<<endl;
    cout<<"******************************************\n"<<endl;*/
    Local+="**********DATA DESCRIPTOR**********\n" "crc32: "+to_string(dataD.crc32)+"\n" +"compressedSize: "+to_string(dataD.compressedSize)+"\n" +"uncompressedSize: "+to_string(dataD.uncompressedSize)+"\n**************************************\n\n";

}

void ZipExplorer::ImprimirCentralDirectory(ifstream &Archivo) {
    Archivo.read ((char*)&CentralHeader, sizeof (CentralDirectory));/*
    cout<<"**************CENTRAL DIRECTORY**************"<<endl;
    cout<<"signature: "<<CentralHeader.signature<<endl;
    cout<<"versionMadeby: "<<CentralHeader.versionMadeby<<endl;
    cout<<"versionToExtract: "<<CentralHeader.versionToExtract<<endl;
    cout<<"generalPurposeBitFlag: "<<CentralHeader.generalPurposeBitFlag<<endl;
    cout<<"compressionMethod: "<<CentralHeader.compressionMethod<<endl;
    cout<<"modificationTime: "<<CentralHeader.modificationTime<<endl;
    cout<<"modificationDate: "<<CentralHeader.modificationDate<<endl;
    cout<<"crc32: "<<CentralHeader.crc32<<endl;
    cout<<"compressedSize: "<<CentralHeader.compressedSize<<endl;
    cout<<"uncompressedSize: "<<CentralHeader.uncompressedSize<<endl;
    cout<<"filenameLength: "<<CentralHeader.filenameLength<<endl;
    cout<<"extraFieldLength: "<<CentralHeader.extraFieldLength<<endl;
    cout<<"fileComentlength: "<<CentralHeader.fileComentlength<<endl;
    cout<<"diskNumberStarts: "<<CentralHeader.diskNumberStarts<<endl;
    cout<<"internalFileAttributes: "<<CentralHeader.internalFileAttributes<<endl;
    cout<<"externalFileAttributes: "<<CentralHeader.externalFileAttributes<<endl;
    cout<<"offsetLocalHeader: "<<CentralHeader.offsetLocalHeader<<endl;*/
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

       Central+="**********CENTRAL DIRECTORY**********\n" "signature: "+to_string(CentralHeader.signature)+"\n" +"versionMadeby: "+to_string(CentralHeader.versionMadeby)+"\n" +"generalPurposeBitFlag: "+to_string(CentralHeader.generalPurposeBitFlag)+"\n" +"compressionMethod: "+to_string(CentralHeader.compressionMethod)+"\n" +"modificationTime: "+to_string(CentralHeader.modificationTime)+"\n" +"modificationDate: "+to_string(CentralHeader.modificationDate)+"\n" +"crc32: "+to_string(CentralHeader.crc32)+"\n" +"compressedSize: "+to_string(CentralHeader.compressedSize)+"\n" +"uncompressedSize: "+to_string(CentralHeader.uncompressedSize)+"\n" +"filenameLength: "+to_string(CentralHeader.filenameLength)+ "\n"+"extraFieldLength: "+to_string(CentralHeader.extraFieldLength)+"\n"+"diskNumberStarts: "+to_string(CentralHeader.diskNumberStarts)+"\n"+"internalFileAttributes: "+to_string(CentralHeader.internalFileAttributes)+"\n"+"externalFileAttributes: "+to_string(CentralHeader.externalFileAttributes)+"\n"+"offsetLocalHeader: "+to_string(CentralHeader.offsetLocalHeader)+"\n**************************************\n\n";
/*
    cout<<"Filename: "<<CentralInfo.Filename<<endl;
    cout<<"ExtraField: "<<CentralInfo.extraField<<endl;
    cout<<"FileComment: "<<CentralInfo.FileComment<<endl;
    cout<<"*********************************************\n"<<endl;*/
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
    Archivo.read ((char*)&EndHeader, sizeof (EndDirectory));/*
    cout<<"**************END DIRECTORY**************"<<endl;
    cout<<"Signature: "<<EndHeader.signature<<endl;
    cout<<"numberDisk: "<<EndHeader.numberDisk<<endl;
    cout<<"DiskWhereStarts: "<<EndHeader.DiskWhereStarts<<endl;
    cout<<"numberCentralDirectory: "<<EndHeader.numberCentralDirectory<<endl;
    cout<<"totalNumberDirectory: "<<EndHeader.totalNumberDirectory<<endl;
    cout<<"sizeofDirectory: "<<EndHeader.sizeofDirectory<<endl;
    cout<<"offsetDirectory: "<<EndHeader.offsetDirectory<<endl;
    cout<<"commentLength: "<<EndHeader.commentLength<<endl;*/

    int espacioR=EndHeader.commentLength;
    char *data=new char[espacioR];

    EndInfo.comment=new char[EndHeader.commentLength];
    Archivo.read(data,EndHeader.commentLength);
    memcpy(EndInfo.comment,data,EndHeader.commentLength);

    End+="**********END DIRECTORY**********\n" "Signature: "+to_string(EndHeader.signature)+"\n" +"numberDisk: "+to_string(EndHeader.numberDisk)+"\n" +"DiskWhereStarts: "+to_string(EndHeader.DiskWhereStarts)+"\n" +"numberCentralDirectory: "+to_string(EndHeader.numberCentralDirectory)+"\n" +"totalNumberDirectory: "+to_string(EndHeader.totalNumberDirectory)+"\n" +"sizeofDirectory: "+to_string(EndHeader.sizeofDirectory)+"\n" +"offsetDirectory: "+to_string(EndHeader.offsetDirectory)+"\n" +"commentLength: "+to_string(EndHeader.commentLength)+"\n" +"commentLength: "+to_string(EndHeader.commentLength)+"\n**************************************\n\n";


    /*
    cout<<"comment: "<<  EndInfo.comment<<endl;
    cout<<"*********************************************"<<endl;*/
}


QStringList ZipExplorer::getZipSystem(){

   return this->zipSystem;
}
