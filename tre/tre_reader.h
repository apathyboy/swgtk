
#ifndef TRE_READER_H_
#define TRE_READER_H_

#include <array>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "tre.h"

namespace tre {

    class TreReader
    {
    public:
        TreReader(std::string filename);

        TreContentsMap ReadIndex();

        const std::string& GetFilename() const;
        const TreHeader& GetHeader() const;

    private:
        void ReadHeader();
                
        std::vector<TreFileInfo> ReadFileBlock();
        std::vector<char> ReadNameBlock();

        typedef std::array<char, 16> Md5Sum;
        std::vector<Md5Sum> ReadMd5SumBlock();
        
        void ValidateFileType(std::string file_type) const;
        void ValidateFileVersion(std::string file_version) const;

        void TreReader::ReadDataBlock(
            uint32_t offset,
            uint32_t compression,
            uint32_t compressed_size, 
            uint32_t uncompressed_size, 
            char* buffer);

        std::ifstream input_stream_;
        std::string filename_;
        TreHeader header_;
    };

}  // namespace tre

#endif  // TRE_READER_H_
