#include "inttypes.h"
#ifndef ZIP_ENDCENTRALDIRECTORY_H
#define ZIP_ENDCENTRALDIRECTORY_H

#pragma  pack(push,1)
struct EndDirectory{
    uint32_t signature;
    uint16_t numberDisk;
    uint16_t DiskWhereStarts;
    uint16_t numberCentralDirectory;
    uint16_t totalNumberDirectory;
    uint32_t sizeofDirectory;
    uint32_t offsetDirectory;
    uint16_t commentLength;
};
struct DataEnd{
    char* comment;
};
#pragma pack(pop)

#endif //ZIP_EXPLORER_ENDCENTRALDIRECTORY_H
