
#include <tre/tre.h>

#include <zlib.h>

using namespace std;
using namespace tre;

std::vector<char> TreDataBlockInfo::ReadDataBlock(std::ifstream* stream)
{
    stream->seekg(offset, ios_base::beg);

    vector<char> data(size);

    if (compression == 0)
    {
        stream->read(&data[0], size);
    }
    else if (compression == 2)
    {
        vector<char> compressed_data(compressed_size);
        stream->read(&compressed_data[0], compressed_size);

        int result = uncompress(
            reinterpret_cast<Bytef*>(&data[0]),
            reinterpret_cast<uLongf*>(&size),
            reinterpret_cast<Bytef*>(&compressed_data[0]),
            compressed_size);

        if (result != Z_OK)
        {
            throw std::runtime_error("ZLib error: " + result);
        }
    }
    else
    {
        throw std::runtime_error("Unknown format");
    }

    return data;
}

