
#include "tre/tre.h"

using namespace std;
using namespace tre;

Tre::Tre(const string& filename)
: filename_(filename)
{
    ifstream file_stream;
    file_stream.exceptions(ifstream::failbit | ifstream::badbit);
    file_stream.open(filename_.c_str(), ios_base::binary);

    header_ = ReadHeader(file_stream);

    file_stream.close();
}

Tre::~Tre()
{}

std::string Tre::GetVersion() const
{
    return header_.version;
}
        
TreHeader Tre::ReadHeader(ifstream& file_stream)
{
    ValidateFileType(file_stream);
    
    TreHeader header;
    
    file_stream.width(4);
    file_stream >> header.version;

    return header;
}

void Tre::ValidateFileType(ifstream& file_stream)
{
    std::string file_type;

    file_stream.width(4);
    file_stream >> file_type;
    
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}
