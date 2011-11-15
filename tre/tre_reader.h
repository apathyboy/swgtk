
#ifndef TRE_TRE_READER_H_
#define TRE_TRE_READER_H_

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <tre/tre.h>

namespace tre {

    class TreReader
    {
    public:
        std::unordered_map<std::string, TreFileInfo> ReadIndex(const std::string& archive_name);
        TreHeader ReadHeader(const std::string& archive_name);

        std::string ReadFileType(std::ifstream& file_stream) const;
        std::string ReadFileVersion(std::ifstream& file_stream) const;

        std::vector<TreFileInfo> ReadFileBlock(TreHeader header, std::ifstream& file_stream) const;
        void ReadFileNames(TreHeader header, std::vector<TreFileInfo>& files, std::ifstream& file_stream) const;
        void ReadMd5Sums(TreHeader header, std::vector<TreFileInfo>& files, std::ifstream& file_stream) const;

        void ValidateFileType(std::string file_type) const;
        void ValidateFileVersion(std::string file_version) const;
    };

}  // namespace tre

#endif  // TRE_TRE_READER_H_
