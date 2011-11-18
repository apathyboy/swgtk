
#include <tre/tre_reader.h>

#include <algorithm>
#include <zlib.h>

#include <ppl.h>

using namespace std;
using namespace tre;
using namespace Concurrency;

TreReader::TreReader(std::string filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
    ReadIndex();
}

void TreReader::ReadIndex()
{
    parallel_invoke(
        [this] { file_block_ = ReadFileBlock(); },
        [this] { name_block_ = ReadNameBlock(); },
        [this] { md5sum_block_ = ReadMd5SumBlock(); }
    );
}

const TreHeader& TreReader::GetHeader() const
{
    return header_;
}

const string& TreReader::GetFilename() const
{
    return filename_;
}

vector<char> TreReader::GetFileData(const string& filename)
{
    auto file_info = GetFileInfo(filename);
    return GetFileData(file_info);
}

vector<char> TreReader::GetFileData(const TreFileInfo& file_info)
{
    vector<char> data(file_info.data_size); 
    
    ReadDataBlock(
        file_info.data_offset, 
        file_info.data_compression, 
        file_info.data_compressed_size, 
        file_info.data_size, 
        &data[0]);

    return data;
}

bool TreReader::ContainsFile(const string& filename)
{
    auto find_iter = find_if(
        begin(file_block_),
        end(file_block_),
        [this, &filename] (const TreFileInfo& info)
    {
        return filename.compare(&name_block_[info.name_offset]) == 0;
    });

    return find_iter != end(file_block_);
}

const TreFileInfo& TreReader::GetFileInfo(const string& filename)
{
    auto find_iter = find_if(
        begin(file_block_),
        end(file_block_),
        [this, &filename] (const TreFileInfo& info)
    {
        return filename.compare(&name_block_[info.name_offset]) == 0;
    });
    
    if (find_iter == end(file_block_))
    {
        throw std::runtime_error("Requested info for invalid file: " + filename);
    }

    return *find_iter;
}

string TreReader::GetMd5Hash(const string& filename)
{
    auto find_iter = find_if(
        begin(file_block_),
        end(file_block_),
        [this, &filename] (const TreFileInfo& info)
    {
        return filename.compare(&name_block_[info.name_offset]) == 0;
    });

    if (find_iter == file_block_.end())
    {
        throw std::runtime_error("File information invalid");
    }
         
    return string(begin(md5sum_block_[find_iter - begin(file_block_)]), 
        end(md5sum_block_[find_iter - begin(file_block_)]));
}

void TreReader::ReadHeader()
{
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

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
    
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.seekg(offset, ios_base::beg);
        input_stream_.read(reinterpret_cast<char*>(&data[0]), size);
    }

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
    if (compression == 0)
    {
        {
            std::lock_guard<std::mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(buffer, uncompressed_size);
        }
    }
    else if (compression == 2)
    {
        vector<char> compressed_data(compressed_size);
        
        {
            std::lock_guard<std::mutex> lg(mutex_);
            input_stream_.seekg(offset, ios_base::beg);
            input_stream_.read(&compressed_data[0], compressed_size);
        }

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
