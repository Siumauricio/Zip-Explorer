#include "Zip.h"
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;
Zip::Zip() {}

void Zip::HeaderFileLocal(string Filename) {
    ifstream Archivo;
    Archivo.open ("//home//siumauricio//Escritorio//"+Filename, ifstream::in | ifstream::binary);
    if(Archivo.fail()){
        return;
    }
    Archivo.seekg (0, ios_base::end);
    int posicionFinal=Archivo.tellg();
    int posicionRecorrido=0;
    Archivo.seekg (0, ios_base::beg);
    while(posicionRecorrido<=posicionFinal){
        Archivo.read ((char*)&Header, sizeof (LocalFileHeader));
        ImprimirLocalHeader(Archivo);
        PrintDataDescriptor(Archivo);
        posicionRecorrido=Archivo.tellg();
    }
    Archivo.close();
}

void Zip::ImprimirLocalHeader(ifstream &Archivo) {

    cout<<"**************LOCAL FILE HEADER**************"<<endl;
    cout<<"Signature: "<<Header.signature<<endl;
    cout<<"versionToExtract: "<<Header.versionToExtract<<endl;
    cout<<"generalPurposeBitFlag: "<<Header.generalPurposeBitFlag<<endl;
    cout<<"compressionMethod: "<<Header.compressionMethod<<endl;
    cout<<"modificationTime: "<<Header.modificationTime<<endl;
    cout<<"modificationDate: "<<Header.modificationDate<<endl;
    cout<<"crc32: "<<Header.crc32<<endl;
    cout<<"compressedSize: "<<Header.compressedSize<<endl;
    cout<<"uncompressedSize: "<<Header.uncompressedSize<<endl;
    cout<<"filenameLength: "<<Header.filenameLength<<endl;
    cout<<"extraFieldLength: "<<Header.extraFieldLength<<endl;
    int espacioR=Header.filenameLength+Header.extraFieldLength;
    char *data=new char[espacioR];

    dataInfo.Filename=new char[Header.filenameLength];
    Archivo.read(data,Header.filenameLength);
    memcpy(dataInfo.Filename,data,Header.filenameLength);

    dataInfo.extraField=new char[Header.extraFieldLength];
    Archivo.read(data,Header.extraFieldLength);
    memcpy(dataInfo.extraField,data,Header.extraFieldLength);

    cout<<"Filename: "<<dataInfo.Filename<<endl;
    cout<<"ExtraField: "<<dataInfo.extraField<<endl;
    cout<<"******************************************\n"<<endl;
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

void Zip::PrintDataDescriptor(ifstream &Archivo) {
    getDataDescriptor(Archivo);
    Archivo.read((char*)&dataD,sizeof(dataDescriptor));
    cout<<"**************DATA DESCRIPTOR**************"<<endl;
    cout<<"crc32: "<<dataD.crc32<<endl;
    cout<<"compressedSize: "<<dataD.compressedSize<<endl;
    cout<<"uncompressedSize: "<<dataD.uncompressedSize<<endl;
    cout<<"******************************************\n"<<endl;
}

