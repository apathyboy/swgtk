
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
        explicit TreFile(const std::string& filename);
        
        bool ContainsResource(const std::string& resource_name) const;
        
        uint32_t GetResourceCount() const;

        std::vector<std::string> GetResourceNames() const;

        const std::string& GetFilename() const;

        uint32_t GetResourceSize(const std::string& resource_name) const;

        std::vector<char> GetResource(const std::string& resource_name);

        std::string GetMd5Hash(const std::string& resource_name) const;
    
    private:
        class TreFileImpl;
        std::shared_ptr<TreFileImpl> impl_;
    };

}  // namespace tre

#endif  // TRE_FILE_H_
