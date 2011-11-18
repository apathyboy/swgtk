
#ifndef TRE_READER_H_
#define TRE_READER_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace tre {

    class TreReader
    {
    public:
        TreReader(std::string filename);
        
        bool ContainsFile(const std::string& filename);
        
        uint32_t GetFileCount() const;
        const std::string& GetFilename() const;
        uint32_t GetFilesize(const std::string& filename);
        std::vector<char> GetFileData(const std::string& filename);
        std::string GetMd5Hash(const std::string& filename);
    
    private:
        class TreReaderImpl;
        std::shared_ptr<TreReaderImpl> impl_;
    };

}  // namespace tre

#endif  // TRE_READER_H_
