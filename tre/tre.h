
#ifndef TRE_TRE_H_
#define TRE_TRE_H_

#include <cstdint>
#include <fstream>
#include <string>

namespace tre 
{
    struct TreHeader
    {
        std::string type;
        std::string version;
    };

    class Tre
    {
    public:
        explicit Tre(const std::string& filename);
        ~Tre();

        std::string GetVersion() const;

        TreHeader ReadHeader(std::ifstream& file_stream) const;
        std::string ReadFileType(std::ifstream& file_stream) const;
        std::string ReadVersion(std::ifstream& file_stream) const;
        void ValidateFileType(std::ifstream& file_stream) const;

    private:
        Tre();

        TreHeader header_;
        std::string filename_;
    };     
}  // namespace tre

#endif  // TRE_TRE_H_
