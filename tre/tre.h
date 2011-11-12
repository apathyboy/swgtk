
#ifndef TRE_TRE_H_
#define TRE_TRE_H_

#include <cstdint>
#include <fstream>
#include <string>

namespace tre 
{
    class Tre
    {
    public:
        explicit Tre(const std::string& filename);
        ~Tre();

        std::string GetVersion() const;

    private:
        Tre();

        void ReadHeader_(std::ifstream& file_stream);

        std::ifstream file_stream_;
        std::string version_;
    };     
}  // namespace tre

#endif  // TRE_TRE_H_
