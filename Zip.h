
#include <string>

using namespace std;
#ifndef ZIP_EXPLORER_ZIP_H
#define ZIP_EXPLORER_ZIP_H

#pragma  pack(push,1)
struct LocalFileHeader{
    uint32_t signature;
    uint16_t versionToExtract;
    uint16_t generalPurposeBitFlag;
    uint16_t compressionMethod;
    uint16_t  modificationTime;
    uint16_t  modificationDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t filenameLength;
    uint16_t extraFieldLength;
    //uint16_t extraField;
};
#pragma pack(pop)
struct Data{
    char* Filename;
    char* extraField;
    char* compressedData;
};

class Zip {
public:
    LocalFileHeader Header;
    Zip();
    void HeaderFileLocal(string);
    void LeerInformacion(string);
    void Imprimir();
};


#endif //ZIP_EXPLORER_ZIP_H
