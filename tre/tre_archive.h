
#ifndef TRE_TRE_ARCHIVE_H_
#define TRE_TRE_ARCHIVE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <tre/tre.h>
#include <tre/tre_reader.h>

namespace tre
{
    class TreFileInterface;

    class TreResourceHandle
    {
    public:
        explicit TreResourceHandle(const TreResourceFile& tre_info);

        const std::vector<char>& GetBuffer() const;
        const std::string& GetFilename() const;
        uint32_t GetFileSize() const;
        const std::string& GetMd5Hash() const;

    private:
        const TreResourceFile& file_info_;
        std::vector<char> buffer_;
    };

    class TreArchive
    {
    public:
        TreArchive();
        explicit TreArchive(const std::string& archive_config);

        void BuildIndex(std::vector<std::string> index_files);

        std::shared_ptr<TreResourceHandle> GetResourceHandle(const std::string& resource_name);

        std::vector<std::string> GetTreList() const;

        std::vector<std::string> ListAvailableResources() const;

    public:
        TreContentsMap tre_index_;
        std::unordered_map<std::string, std::shared_ptr<TreResourceHandle>> resource_handles_;
        std::vector<std::unique_ptr<TreReader>> tre_list_;
    };
}

#endif  // TRE_TRE_ARCHIVE_H_
