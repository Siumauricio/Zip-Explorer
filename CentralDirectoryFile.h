#include <inttypes.h>
#ifndef ZIP_CENTRALDIRECTORYFILE_H
#define ZIP_CENTRALDIRECTORYFILE_H

#pragma  pack(push,1)
struct CentralDirectory{
    uint32_t signature;
    uint16_t versionMadeby;
    uint16_t versionToExtract;
    uint16_t generalPurposeBitFlag;
    uint16_t  compressionMethod;
    uint16_t  modificationTime;
    uint16_t  modificationDate;
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
    uint16_t filenameLength;
    uint16_t extraFieldLength;
    uint16_t fileComentlength;
    uint16_t diskNumberStarts;
    uint16_t internalFileAttributes;
    uint32_t externalFileAttributes;
    uint32_t offsetLocalHeader;
};
struct DataCentral{
    char* Filename;
    char* extraField;
    char* FileComment;
};

#pragma pack(pop)

#endif //ZIP_EXPLORER_CENTRALDIRECTORYFILE_H
