
#include <tre/tre_reader.h>

#include <algorithm>
#include <zlib.h>

using namespace std;
using namespace tre;

TreReader::TreReader(std::string filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
}

TreContentsMap TreReader::ReadIndex()
{    
    vector<TreFileInfo> file_block = ReadFileBlock();
    vector<char> name_block = ReadNameBlock();
    vector<Md5Sum> md5_sum_block = ReadMd5SumBlock();

    TreContentsMap index;

    transform(
        begin(file_block),
        end(file_block),
        begin(md5_sum_block),
        inserter(index, index.begin()),
        [&name_block] (TreFileInfo& file_info, Md5Sum& md5_sum) -> TreContentsMap::value_type
    {
        TreResourceFile resource_file;
        resource_file.filename = string(&name_block[file_info.name_offset]);
        resource_file.md5sum = string(begin(md5_sum), end(md5_sum));
        resource_file.info = move(file_info);

        return make_pair(resource_file.filename, move(resource_file));
    });

    return index;
}

const TreHeader& TreReader::GetHeader() const
{
    return header_;
}

const string& TreReader::GetFilename() const
{
    return filename_;
}

void TreReader::ReadHeader()
{
    input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));

    ValidateFileType(string(header_.file_type, 4));
    ValidateFileVersion(string(header_.file_version, 4));        
}
        
vector<TreFileInfo> TreReader::ReadFileBlock()
{
    uint32_t uncompressed_size = header_.file_count * sizeof(TreFileInfo);
    
    vector<TreFileInfo> files(header_.file_count);
        
    ReadDataBlock(header_.info_offset,
        header_.info_compression,
        header_.info_compressed_size,
        uncompressed_size,
        reinterpret_cast<char*>(&files[0]));

    return files;
}
        
vector<char> TreReader::ReadNameBlock()
{
    vector<char> data(header_.name_uncompressed_size); 
    
    uint32_t name_offset = header_.info_offset + header_.info_compressed_size;

    ReadDataBlock(
        name_offset, 
        header_.name_compression, 
        header_.name_compressed_size, 
        header_.name_uncompressed_size, 
        &data[0]);

    return data;
}
        
vector<TreReader::Md5Sum> TreReader::ReadMd5SumBlock()
{    
    uint32_t offset = header_.info_offset
        + header_.info_compressed_size
        + header_.name_compressed_size;
    uint32_t size = header_.file_count * 16; // where 16 is the length of a md5 sum
        
    vector<Md5Sum> data(size);
    
    input_stream_.seekg(offset, ios_base::beg);
    input_stream_.read(reinterpret_cast<char*>(&data[0]), size);

    return data;
}

void TreReader::ValidateFileType(string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}

void TreReader::ValidateFileVersion(string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw runtime_error("Invalid tre file version");
    }
}

void TreReader::ReadDataBlock(
    uint32_t offset,
    uint32_t compression,
    uint32_t compressed_size, 
    uint32_t uncompressed_size, 
    char* buffer)
{
    input_stream_.seekg(offset, ios_base::beg);
    
    if (compression == 0)
    {
        input_stream_.read(buffer, uncompressed_size);
    }
    else if (compression == 2)
    {
        vector<char> compressed_data(compressed_size);
        input_stream_.read(&compressed_data[0], compressed_size);

        int result = uncompress(
            reinterpret_cast<Bytef*>(buffer),
            reinterpret_cast<uLongf*>(&uncompressed_size),
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
}
