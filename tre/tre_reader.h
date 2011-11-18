
#ifndef TRE_READER_H_
#define TRE_READER_H_

#include <array>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <mutex>
#include <concurrent_vector.h>

#include "tre.h"

namespace tre {

    class TreReader
    {
    public:
        TreReader(std::string filename);
        
        bool ContainsFile(const std::string& filename);

        const std::string& GetFilename() const;
        const TreHeader& GetHeader() const;

        std::vector<char> GetFileData(const TreFileInfo& file_info);
        std::vector<char> GetFileData(const std::string& filename);
        const TreFileInfo& GetFileInfo(const std::string& filename);
        std::string GetMd5Hash(const std::string& filename);
        
    private:
        void ReadHeader();
        void ReadIndex();
                
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

        std::mutex mutex_;

        std::vector<TreFileInfo> file_block_;
        std::vector<char> name_block_;
        std::vector<Md5Sum> md5sum_block_;
    };

}  // namespace tre

#endif  // TRE_READER_H_
