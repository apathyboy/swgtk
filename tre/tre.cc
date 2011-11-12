
#include "tre/tre.h"

using namespace std;
using namespace tre;

Tre::Tre(const string& filename)
{
    file_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    file_stream_.open(filename.c_str(), ios_base::binary);

    ReadHeader_(file_stream_);
}

Tre::~Tre()
{
    file_stream_.close();
}

std::string Tre::GetVersion() const
{
    return version_;
}
        
void Tre::ReadHeader_(ifstream& file_stream)
{
    std::string file_type;

    file_stream.width(4);
    file_stream >> file_type;

    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
    
    file_stream.width(4);
    file_stream >> version_;
}