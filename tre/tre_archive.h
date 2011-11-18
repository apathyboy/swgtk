
#ifndef TRE_TRE_ARCHIVE_H_
#define TRE_TRE_ARCHIVE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <concurrent_unordered_map.h>

#include <tre/tre.h>
#include <tre/tre_reader.h>

namespace tre
{
    class TreFileInterface;

    class TreResourceHandle
    {
    public:
        explicit TreResourceHandle(const std::string& filename, TreReader* tre_reader);

        const std::vector<char>& GetBuffer() const;
        std::string GetFilename() const;
        uint32_t GetFileSize() const;
        std::string GetMd5Hash() const;

    private:
        std::string filename_;
        TreFileInfo file_info_;
        TreReader* tre_reader_;
        std::vector<char> buffer_;
    };

    class TreArchive
    {
    public:
        TreArchive();
        explicit TreArchive(const std::string& archive_config);

        void BuildIndex(std::vector<std::string> index_files);

        std::shared_ptr<TreResourceHandle> GetResourceHandle(const std::string& resource_name);

        const std::vector<std::string>& GetTreFilenames() const;

        std::vector<std::string> ListAvailableResources() const;

    public:
        typedef Concurrency::concurrent_unordered_map<
            std::string, 
            std::shared_ptr<TreResourceHandle>
        > ResourceHandleMap;
        ResourceHandleMap resource_handles_;

        typedef std::unordered_map<
            std::string, 
            std::unique_ptr<TreReader>
        > TreReaderMap;
        TreReaderMap tre_list_;

        std::vector<std::string> tre_filenames_;
    };
}

#endif  // TRE_TRE_ARCHIVE_H_
