
#include <tre/tre_reader.h>

#include <array>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <zlib.h>

#include <ppl.h>

#include "tre.h"

using namespace std;
using namespace tre;
using namespace Concurrency;


class TreReader::TreReaderImpl
{
public:
        TreReaderImpl(std::string filename);
        
        bool ContainsFile(const std::string& filename);
        
        uint32_t GetFileCount() const;

        const std::string& GetFilename() const;

        std::vector<char> GetFileData(const TreFileInfo& file_info);
        std::vector<char> GetFileData(const std::string& filename);
        std::string GetMd5Hash(const std::string& filename);
        uint32_t GetFilesize(const std::string& filename);

    const TreFileInfo& GetFileInfo(const std::string& filename);

    void ReadHeader();
    void ReadIndex();
            
    std::vector<TreFileInfo> ReadFileBlock();
    std::vector<char> ReadNameBlock();
    
    const TreHeader& GetHeader() const;

    typedef std::array<char, 16> Md5Sum;
    std::vector<Md5Sum> ReadMd5SumBlock();
    
    void ValidateFileType(std::string file_type) const;
    void ValidateFileVersion(std::string file_version) const;

    void ReadDataBlock(
        uint32_t offset,
        uint32_t compression,
        uint32_t compressed_size, 
        uint32_t uncompressed_size, 
        char* buffer);

private:
        std::ifstream input_stream_;
        std::string filename_;
        TreHeader header_;

        std::mutex mutex_;

        std::vector<TreFileInfo> file_block_;
        std::vector<char> name_block_;
        std::vector<Md5Sum> md5sum_block_;
};

TreReader::TreReader(std::string filename)
: impl_(new TreReaderImpl(filename))
{}

uint32_t TreReader::GetFileCount() const
{
    return impl_->GetFileCount();
}

const string& TreReader::GetFilename() const
{
    return impl_->GetFilename();
}

vector<char> TreReader::GetFileData(const string& filename)
{
    return impl_->GetFileData(filename);
}

bool TreReader::ContainsFile(const string& filename)
{
    return impl_->ContainsFile(filename);
}

string TreReader::GetMd5Hash(const string& filename)
{
    return impl_->GetMd5Hash(filename);
}

uint32_t TreReader::GetFilesize(const string& filename)
{
    return impl_->GetFilesize(filename);
}

TreReader::TreReaderImpl::TreReaderImpl(std::string filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
    ReadIndex();
}

uint32_t TreReader::TreReaderImpl::GetFileCount() const
{
    return header_.file_count;
}

const string& TreReader::TreReaderImpl::GetFilename() const
{
    return filename_;
}

vector<char> TreReader::TreReaderImpl::GetFileData(const string& filename)
{
    auto file_info = GetFileInfo(filename);
    return GetFileData(file_info);
}

vector<char> TreReader::TreReaderImpl::GetFileData(const TreFileInfo& file_info)
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

bool TreReader::TreReaderImpl::ContainsFile(const string& filename)
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

string TreReader::TreReaderImpl::GetMd5Hash(const string& filename)
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
        throw std::runtime_error("File name invalid");
    }
         
    return string(begin(md5sum_block_[find_iter - begin(file_block_)]), 
        end(md5sum_block_[find_iter - begin(file_block_)]));
}

uint32_t TreReader::TreReaderImpl::GetFilesize(const string& filename)
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
        throw std::runtime_error("File name invalid");
    }
         
    return find_iter->data_size;
}

const TreHeader& TreReader::TreReaderImpl::GetHeader() const
{
    return header_;
}

const TreFileInfo& TreReader::TreReaderImpl::GetFileInfo(const string& filename)
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

void TreReader::TreReaderImpl::ReadHeader()
{
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

    ValidateFileType(string(header_.file_type, 4));
    ValidateFileVersion(string(header_.file_version, 4));        
}

void TreReader::TreReaderImpl::ReadIndex()
{
    parallel_invoke(
        [this] { file_block_ = ReadFileBlock(); },
        [this] { name_block_ = ReadNameBlock(); },
        [this] { md5sum_block_ = ReadMd5SumBlock(); }
    );
}

vector<TreFileInfo> TreReader::TreReaderImpl::ReadFileBlock()
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
        
vector<char> TreReader::TreReaderImpl::ReadNameBlock()
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
        
vector<TreReader::TreReaderImpl::Md5Sum> TreReader::TreReaderImpl::ReadMd5SumBlock()
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

void TreReader::TreReaderImpl::ValidateFileType(string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}

void TreReader::TreReaderImpl::ValidateFileVersion(string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw runtime_error("Invalid tre file version");
    }
}

void TreReader::TreReaderImpl::ReadDataBlock(
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
