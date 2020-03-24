#include <string>
#include "localfileheader.h"
#include "centraldirectoryfile.h"
#include "endcentraldirectory.h"
#include <QStringList>
using namespace std;
#ifndef ZIP_EXPLORER_H
#define ZIP_EXPLORER_H


class ZipExplorer {
private:
    //LOCAL HEADER
    LocalFileHeader LocalHeader;
    DataLocal LocalInfo;
    DataDescriptor dataD;
    //CENTRAL HEADER
    CentralDirectory CentralHeader;
    DataCentral CentralInfo;
    //END DIRECTORY
    EndDirectory EndHeader;
    DataEnd EndInfo;
    int cantidadDirs;
    //GENERAL
    QStringList zipSystem;
public:
    bool isDeflate;
    bool hasData;
    string Local;
    string Central;
    string End;
    ZipExplorer();
    ~ZipExplorer();
    void HeaderFileLocal(string);
    uint8_t getDataDescriptor(ifstream&);
    bool isDirectory();
    bool isFile();
    QStringList getZipSystem();
    void ImprimirLocalHeader(ifstream &);
    void ImprimirCentralDirectory(ifstream&,string);
    void ImprimirEndDirectory(ifstream&);
    void ImprimirDataDescriptor(ifstream&);
};
#endif //ZIP_EXPLORER_H

