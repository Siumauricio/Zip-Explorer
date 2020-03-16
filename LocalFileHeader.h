#ifndef ZIP_LOCALFILEHEADER_H
#define ZIP_LOCALFILEHEADER_H
#include <cinttypes>

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
};
struct DataDescriptor{
    uint32_t crc32;
    uint32_t compressedSize;
    uint32_t uncompressedSize;
};
struct DataLocal{
    char* Filename;
    char* extraField;
};
#pragma pack(pop)

#endif //ZIP_EXPLORER_LOCALFILEHEADER_H
