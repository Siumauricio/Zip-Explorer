#include "Zip.h"
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>


int x1=0;
using namespace std;
Zip::Zip() {cantidadDirs=0;}

void Zip::HeaderFileLocal(string Filename) {
    ifstream Archivo;
    Archivo.open ("//home//siumauricio//Escritorio//"+Filename, ifstream::in | ifstream::binary);
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



void Zip::ImprimirLocalHeader(ifstream &Archivo) {
    Archivo.read ((char*)&LocalHeader, sizeof (LocalFileHeader));
    if(LocalHeader.signature!=67324752){
        LocalInfo.Filename="";
        cout<<"pase por aqui"<<endl;
        cantidadDirs++;
        return;
    }
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
        cout<<"extraFieldLength: "<<LocalHeader.extraFieldLength<<endl;
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

        cout<<"Filename: "<<LocalInfo.Filename<<endl;
        cout<<"ExtraField: "<<LocalInfo.extraField<<endl;
        cout<<"******************************************\n"<<endl;

        cantidadDirs++;

}

void Zip::getDataDescriptor(ifstream &Archivo) {
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
                        break;
                    }
                }
            }
        }
    }
}

void Zip::ImprimirDataDescriptor(ifstream &Archivo) {
    getDataDescriptor(Archivo);
    Archivo.read((char*)&dataD,sizeof(DataDescriptor));
    cout<<"**************DATA DESCRIPTOR**************"<<endl;
    cout<<"crc32: "<<dataD.crc32<<endl;
    cout<<"compressedSize: "<<dataD.compressedSize<<endl;
    cout<<"uncompressedSize: "<<dataD.uncompressedSize<<endl;
    cout<<"******************************************\n"<<endl;
}

void Zip::ImprimirCentralDirectory(ifstream &Archivo) {
    Archivo.read ((char*)&CentralHeader, sizeof (CentralDirectory));
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
    cout<<"offsetLocalHeader: "<<CentralHeader.offsetLocalHeader<<endl;
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

    cout<<"Filename: "<<CentralInfo.Filename<<endl;
    cout<<"ExtraField: "<<CentralInfo.extraField<<endl;
    cout<<"FileComment: "<<CentralInfo.FileComment<<endl;
    cout<<"*********************************************\n"<<endl;
}

bool Zip::isDirectory() {

    string filename=LocalInfo.Filename;
    return filename[filename.length()-1] == '/';
}

bool Zip::isFile() {
    string filename=LocalInfo.Filename;
    return filename[filename.length()-1] != '/';
}

void Zip::ImprimirEndDirectory(ifstream &Archivo) {
    Archivo.read ((char*)&EndHeader, sizeof (EndDirectory));
    cout<<"**************END DIRECTORY**************"<<endl;
    cout<<"Signature: "<<EndHeader.signature<<endl;
    cout<<"numberDisk: "<<EndHeader.numberDisk<<endl;
    cout<<"DiskWhereStarts: "<<EndHeader.DiskWhereStarts<<endl;
    cout<<"numberCentralDirectory: "<<EndHeader.numberCentralDirectory<<endl;
    cout<<"totalNumberDirectory: "<<EndHeader.totalNumberDirectory<<endl;
    cout<<"sizeofDirectory: "<<EndHeader.sizeofDirectory<<endl;
    cout<<"offsetDirectory: "<<EndHeader.offsetDirectory<<endl;
    cout<<"commentLength: "<<EndHeader.commentLength<<endl;

    int espacioR=EndHeader.commentLength;
    char *data=new char[espacioR];

    EndInfo.comment=new char[EndHeader.commentLength];
    Archivo.read(data,EndHeader.commentLength);
    memcpy(EndInfo.comment,data,EndHeader.commentLength);
    cout<<"comment: "<<  EndInfo.comment<<endl;
    cout<<"*********************************************"<<endl;
}
