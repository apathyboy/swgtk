
#ifndef TRE_H_
#define TRE_H_

#include <cstdint>
#include <string>
#include <unordered_map>

namespace tre {

#pragma pack(1)
    struct TreHeader
    {
        char file_type[4];
        char file_version[4];
        uint32_t file_count;
        uint32_t info_offset;
        uint32_t info_compression;
        uint32_t info_compressed_size;
        uint32_t name_compression;
        uint32_t name_compressed_size;
        uint32_t name_uncompressed_size;
    };
    
    struct TreFileInfo
    {
        uint32_t checksum;
        uint32_t data_size;
        uint32_t data_offset;
        uint32_t data_compression;
        uint32_t data_compressed_size;
        uint32_t name_offset;
    };
#pragma pack()

}  // namespace tre

#endif  // TRE_H_

