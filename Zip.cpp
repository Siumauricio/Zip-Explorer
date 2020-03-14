#include "Zip.h"
#include <fstream>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;
Zip::Zip() {}

void Zip::HeaderFileLocal(string Filename) {
    cout<<endl;
    ifstream Archivo;
    Archivo.open ("//home//siumauricio//Escritorio//"+Filename, ifstream::in | ifstream::binary);
    if(Archivo.fail()){
        return;
    }
    Archivo.seekg (0, ios_base::beg);
    Archivo.read ((char*)&Header, sizeof (LocalFileHeader));
    Archivo.close();
    Imprimir();
}

void Zip::Imprimir() {
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
}


