
#ifndef TRE_TRE_INFO_H_
#define TRE_TRE_INFO_H_

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace tre {
    
    struct TreDataBlockInfo
    {
        uint32_t offset;
        uint32_t compression;
        uint32_t compressed_size;
        uint32_t size;

        std::vector<char> ReadDataBlock(std::ifstream* stream);
    };

    struct TreFileInfo
    {
        std::string filename;
        std::string md5sum;
        uint32_t checksum;
        uint32_t name_offset;
        TreDataBlockInfo file_data;
    };
    
    struct TreHeader
    {
        std::string name;
        uint32_t record_count;
        TreDataBlockInfo file_info;
        TreDataBlockInfo file_name;
    };

}  // namespace tre

#endif  // TRE_TRE_INFO_H_

