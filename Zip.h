#include <string>
#include "LocalFileHeader.h"
#include "CentralDirectoryFile.h"
#include "EndCentralDirectory.h"

using namespace std;
#ifndef ZIP_EXPLORER_H
#define ZIP_EXPLORER_H

class Zip {
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
public:
    Zip();
    void HeaderFileLocal(string);
    void getDataDescriptor(ifstream&);
    bool isDirectory();
    bool isFile();

    void ImprimirLocalHeader(ifstream &);
    void ImprimirCentralDirectory(ifstream&);
    void ImprimirEndDirectory(ifstream&);
    void ImprimirDataDescriptor(ifstream&);
};

#endif //ZIP_EXPLORER_H
