
#ifndef TRE_TRE_ARCHIVE_H_
#define TRE_TRE_ARCHIVE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace tre
{
    class TreFileInterface;

    class TreResourceHandle
    {
    public:
        const std::vector<char>& GetBuffer() const;
        const std::string& GetFilename() const;
        const std::string& GetTreFilename() const;
        uint32_t GetFileSize() const;
        uint32_t GetCompressedFileSize() const;
        const std::string& GetMd5Hash() const;

    private:
        std::vector<char> buffer_;
        std::string filename_;
        std::string tre_filename_;
        std::string md5_hash_;
        uint32_t file_size_;
        uint32_t compressed_file_size_;
    };

    class TreArchive
    {
    public:
        TreArchive();
        explicit TreArchive(const std::string& archive_config);

        void BuildIndex(std::vector<std::string> index_files);

        std::shared_ptr<TreResourceHandle> GetResourceHandle(const std::string& resource_name);

        std::vector<std::string> GetTreList() const;

    public:
        std::vector<std::string> tre_list_;
    };
}

#endif  // TRE_TRE_ARCHIVE_H_
