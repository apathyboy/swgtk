
#ifndef TRE_TRE_ARCHIVE_H_
#define TRE_TRE_ARCHIVE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace tre
{
    class TreFile;

    class TreArchive
    {
    public:
        explicit TreArchive(std::vector<std::string> index_files);

        uint32_t GetFilesize(const std::string& filename) const;

        std::vector<char> GetFileData(const std::string& filename);

        std::string GetMd5Hash(const std::string& filename) const;

        const std::vector<std::string>& GetTreFilenames() const;

        std::vector<std::string> GetAvailableFiles() const;

    public:
        TreArchive();

        typedef std::unordered_map<
            std::string, 
            std::shared_ptr<TreFile>
        > TreFileMap;
        TreFileMap tre_list_;

        std::vector<std::string> tre_filenames_;
    };
}

#endif  // TRE_TRE_ARCHIVE_H_
