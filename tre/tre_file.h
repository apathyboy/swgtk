
#ifndef TRE_FILE_H_
#define TRE_FILE_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace tre {

    class TreFile
    {
    public:
        TreFile(std::string filename);
        
        bool ContainsFile(const std::string& filename) const;
        
        uint32_t GetFileCount() const;
        const std::string& GetFilename() const;
        std::vector<std::string> GetFilenames() const;
        uint32_t GetFileSize(const std::string& filename) const;
        std::vector<char> GetFileData(const std::string& filename);
        std::string GetMd5Hash(const std::string& filename) const;
    
    private:
        class TreFileImpl;
        std::shared_ptr<TreFileImpl> impl_;
    };

}  // namespace tre

#endif  // TRE_FILE_H_
