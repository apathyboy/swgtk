
#include <tre/tre_file.h>

#include <array>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <zlib.h>

#include <ppl.h>

using namespace std;
using namespace tre;
using namespace Concurrency;

#pragma pack(1)
    struct TreHeader
    {
        char file_type[4];
        char file_version[4];
        uint32_t file_count;
        uint32_t info_offset;
        uint32_t info_compression;
        uint32_t info_compressed_size;
        uint32_t name_compression;
        uint32_t name_compressed_size;
        uint32_t name_uncompressed_size;
    };
    
    struct TreFileInfo
    {
        uint32_t checksum;
        uint32_t data_size;
        uint32_t data_offset;
        uint32_t data_compression;
        uint32_t data_compressed_size;
        uint32_t name_offset;
    };
#pragma pack()

class TreFile::TreFileImpl
{
public:
        TreFileImpl(std::string filename);
        
        bool ContainsFile(const std::string& filename) const;
        
        uint32_t GetFileCount() const;

        const std::string& GetFilename() const;
        std::vector<std::string> GetFilenames() const;

        std::vector<char> GetFileData(const TreFileInfo& file_info);
        std::vector<char> GetFileData(const std::string& filename);
        std::string GetMd5Hash(const std::string& filename) const;
        uint32_t GetFileSize(const std::string& filename) const;

    const TreFileInfo& GetFileInfo(const std::string& filename) const;

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

TreFile::TreFile(std::string filename)
: impl_(new TreFileImpl(filename))
{}

uint32_t TreFile::GetFileCount() const
{
    return impl_->GetFileCount();
}

vector<string> TreFile::GetFilenames() const
{
    return impl_->GetFilenames();
}

const string& TreFile::GetFilename() const
{
    return impl_->GetFilename();
}

vector<char> TreFile::GetFileData(const string& filename)
{
    return impl_->GetFileData(filename);
}

bool TreFile::ContainsFile(const string& filename) const
{
    return impl_->ContainsFile(filename);
}

string TreFile::GetMd5Hash(const string& filename) const
{
    return impl_->GetMd5Hash(filename);
}

uint32_t TreFile::GetFileSize(const string& filename) const
{
    return impl_->GetFileSize(filename);
}

TreFile::TreFileImpl::TreFileImpl(std::string filename)
: filename_(filename)
{
    input_stream_.exceptions(ifstream::failbit | ifstream::badbit);
    input_stream_.open(filename_.c_str(), ios_base::binary);

    ReadHeader();
    ReadIndex();
}

uint32_t TreFile::TreFileImpl::GetFileCount() const
{
    return header_.file_count;
}

const string& TreFile::TreFileImpl::GetFilename() const
{
    return filename_;
}

std::vector<std::string> TreFile::TreFileImpl::GetFilenames() const
{
    std::vector<std::string> filenames;

    for_each(
        begin(file_block_),
        end(file_block_),
        [this, &filenames] (const TreFileInfo& info)
    {
        filenames.push_back(&name_block_[info.name_offset]);
    });

    return filenames;
}

vector<char> TreFile::TreFileImpl::GetFileData(const string& filename)
{
    auto file_info = GetFileInfo(filename);
    return GetFileData(file_info);
}

vector<char> TreFile::TreFileImpl::GetFileData(const TreFileInfo& file_info)
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

bool TreFile::TreFileImpl::ContainsFile(const string& filename) const
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

string TreFile::TreFileImpl::GetMd5Hash(const string& filename) const
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

    stringstream ss;

    ss.flags(ss.hex);
    ss.fill('0');
    ss.width(2);
    
    for_each(
        begin(md5sum_block_[find_iter - begin(file_block_)]), 
        begin(md5sum_block_[find_iter - begin(file_block_)]) + sizeof(Md5Sum),
        [&ss] (unsigned char c) 
    {
        ss << static_cast<unsigned>(c);
    });

    return ss.str();
}

uint32_t TreFile::TreFileImpl::GetFileSize(const string& filename) const
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

const TreHeader& TreFile::TreFileImpl::GetHeader() const
{
    return header_;
}

const TreFileInfo& TreFile::TreFileImpl::GetFileInfo(const string& filename) const
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

void TreFile::TreFileImpl::ReadHeader()
{
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

    ValidateFileType(string(header_.file_type, 4));
    ValidateFileVersion(string(header_.file_version, 4));        
}

void TreFile::TreFileImpl::ReadIndex()
{
    parallel_invoke(
        [this] { file_block_ = ReadFileBlock(); },
        [this] { name_block_ = ReadNameBlock(); },
        [this] { md5sum_block_ = ReadMd5SumBlock(); }
    );
}

vector<TreFileInfo> TreFile::TreFileImpl::ReadFileBlock()
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
        
vector<char> TreFile::TreFileImpl::ReadNameBlock()
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
        
vector<TreFile::TreFileImpl::Md5Sum> TreFile::TreFileImpl::ReadMd5SumBlock()
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

void TreFile::TreFileImpl::ValidateFileType(string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw runtime_error("Invalid tre file format");
    }
}

void TreFile::TreFileImpl::ValidateFileVersion(string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw runtime_error("Invalid tre file version");
    }
}

void TreFile::TreFileImpl::ReadDataBlock(
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
