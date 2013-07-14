
#include "tre_reader.h"

#include <array>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <zlib.h>

#include "tre_data.h"

using namespace swgtk;

class TreReader::TreReaderImpl
{
public:
    TreReaderImpl(std::string filename);
    
    void Initialize();

    bool IsInitialized() const;

    bool ContainsResource(const std::string& resource_name) const;
    
    uint32_t GetResourceCount() const;

    const std::string& GetFilename() const;
    std::vector<std::string> GetResourceNames() const;

    std::vector<char> GetResource(const TreResourceInfo& resource_info);
    std::string GetMd5Hash(const std::string& resource_name) const;
    uint32_t GetResourceSize(const std::string& resource_name) const;

    const TreResourceInfo& GetResourceInfo(const std::string& resource_name) const;

private:
    TreReaderImpl();

    void ReadHeader();
    void ReadIndex();
            
    std::vector<TreResourceInfo> ReadResourceBlock();
    std::vector<char> ReadNameBlock();

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

    std::ifstream input_stream_;
    std::string filename_;
    TreHeader header_;

    bool initialized_;

    std::mutex mutex_;

    std::vector<TreResourceInfo> resource_block_;
    std::vector<char> name_block_;
    std::vector<Md5Sum> md5sum_block_;
};

TreReader::TreReader()
: impl_(nullptr)
{}

TreReader::TreReader(const std::string& filename)
: impl_(new TreReaderImpl(filename))
{}

void TreReader::Initialize()
{
    impl_->Initialize();
}

uint32_t TreReader::GetResourceCount() const
{
    VerifyInitialization();

    return impl_->GetResourceCount();
}

std::vector<std::string> TreReader::GetResourceNames() const
{
    VerifyInitialization();

    return impl_->GetResourceNames();
}

const std::string& TreReader::GetFilename() const
{
    VerifyInitialization();

    return impl_->GetFilename();
}

std::vector<char> TreReader::GetResource(const std::string& resource_name)
{
    VerifyInitialization();

    return impl_->GetResource(
		impl_->GetResourceInfo(resource_name));
}

bool TreReader::ContainsResource(const std::string& resource_name) const
{
    VerifyInitialization();

    return impl_->ContainsResource(resource_name);
}

std::string TreReader::GetMd5Hash(const std::string& resource_name) const
{
    VerifyInitialization();

    return impl_->GetMd5Hash(resource_name);
}

uint32_t TreReader::GetResourceSize(const std::string& resource_name) const
{
    VerifyInitialization();

    return impl_->GetResourceSize(resource_name);
}

void TreReader::VerifyInitialization() const
{
    if (!impl_->IsInitialized()) throw std::runtime_error("Attempted to use TreReader without initializing");
}

TreReader::TreReaderImpl::TreReaderImpl(std::string filename)
: filename_(filename)
, initialized_(false)
{
    input_stream_.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    input_stream_.open(filename_.c_str(), std::ios_base::binary);

    ReadHeader();
}

void TreReader::TreReaderImpl::Initialize()
{
    if (!IsInitialized())
    {
        ReadIndex();
        initialized_ = true;
    }
}

bool TreReader::TreReaderImpl::IsInitialized() const
{
    return initialized_;
}

uint32_t TreReader::TreReaderImpl::GetResourceCount() const
{
    return header_.resource_count;
}

const std::string& TreReader::TreReaderImpl::GetFilename() const
{
    return filename_;
}

std::vector<std::string> TreReader::TreReaderImpl::GetResourceNames() const
{
    std::vector<std::string> resource_names;

    std::for_each(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_names] (const TreResourceInfo& info)
    {
        resource_names.push_back(&name_block_[info.name_offset]);
    });

    return resource_names;
}

std::vector<char> TreReader::TreReaderImpl::GetResource(const TreResourceInfo& file_info)
{
    std::vector<char> data(file_info.data_size); 
    
    ReadDataBlock(
        file_info.data_offset, 
        file_info.data_compression, 
        file_info.data_compressed_size, 
        file_info.data_size, 
        &data[0]);

    return data;
}

bool TreReader::TreReaderImpl::ContainsResource(const std::string& resource_name) const
{
    auto find_iter = std::find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });

    return find_iter != end(resource_block_);
}

std::string TreReader::TreReaderImpl::GetMd5Hash(const std::string& resource_name) const
{
    auto find_iter = std::find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });

    if (find_iter == resource_block_.end())
    {
        throw std::runtime_error("File name invalid");
    }

    std::stringstream ss;

    ss.flags(ss.hex);
    ss.fill('0');
    ss.width(2);
    
    std::for_each(
        std::begin(md5sum_block_[find_iter - std::begin(resource_block_)]), 
        std::begin(md5sum_block_[find_iter - std::begin(resource_block_)]) + sizeof(Md5Sum),
        [&ss] (unsigned char c) 
    {
        ss << static_cast<unsigned>(c);
    });

    return ss.str();
}

uint32_t TreReader::TreReaderImpl::GetResourceSize(const std::string& resource_name) const
{
    auto find_iter = std::find_if(
        begin(resource_block_),
        end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });

    if (find_iter == resource_block_.end())
    {
        throw std::runtime_error("File name invalid");
    }
         
    return find_iter->data_size;
}

const TreResourceInfo& TreReader::TreReaderImpl::GetResourceInfo(const std::string& resource_name) const
{
    auto find_iter = std::find_if(
        std::begin(resource_block_),
        std::end(resource_block_),
        [this, &resource_name] (const TreResourceInfo& info)
    {
        return resource_name.compare(&name_block_[info.name_offset]) == 0;
    });
    
    if (find_iter == std::end(resource_block_))
    {
        throw std::runtime_error("Requested info for invalid file: " + resource_name);
    }

    return *find_iter;
}

void TreReader::TreReaderImpl::ReadHeader()
{
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.read(reinterpret_cast<char*>(&header_), sizeof(header_));
    }

    ValidateFileType(std::string(header_.file_type, 4));
    ValidateFileVersion(std::string(header_.file_version, 4));        
}

void TreReader::TreReaderImpl::ReadIndex()
{
    resource_block_ = ReadResourceBlock();
    name_block_ = ReadNameBlock();
    md5sum_block_ = ReadMd5SumBlock();
}

std::vector<TreResourceInfo> TreReader::TreReaderImpl::ReadResourceBlock()
{
    uint32_t uncompressed_size = header_.resource_count * sizeof(TreResourceInfo);
    
    std::vector<TreResourceInfo> files(header_.resource_count);
        
    ReadDataBlock(header_.info_offset,
        header_.info_compression,
        header_.info_compressed_size,
        uncompressed_size,
        reinterpret_cast<char*>(&files[0]));

    return files;
}
        
std::vector<char> TreReader::TreReaderImpl::ReadNameBlock()
{
    std::vector<char> data(header_.name_uncompressed_size); 
    
    uint32_t name_offset = header_.info_offset + header_.info_compressed_size;

    ReadDataBlock(
        name_offset, 
        header_.name_compression, 
        header_.name_compressed_size, 
        header_.name_uncompressed_size, 
        &data[0]);

    return data;
}
        
std::vector<TreReader::TreReaderImpl::Md5Sum> TreReader::TreReaderImpl::ReadMd5SumBlock()
{    
    uint32_t offset = header_.info_offset
        + header_.info_compressed_size
        + header_.name_compressed_size;
    uint32_t size = header_.resource_count * 16; // where 16 is the length of a md5 sum
        
    std::vector<Md5Sum> data(size);
    
    {
        std::lock_guard<std::mutex> lg(mutex_);
        input_stream_.seekg(offset, std::ios_base::beg);
        input_stream_.read(reinterpret_cast<char*>(&data[0]), size);
    }

    return data;
}

void TreReader::TreReaderImpl::ValidateFileType(std::string file_type) const
{
    if (file_type.compare("EERT") != 0)
    {
        throw std::runtime_error("Invalid tre file format");
    }
}

void TreReader::TreReaderImpl::ValidateFileVersion(std::string file_version) const
{
    if (file_version.compare("5000") != 0)
    {
        throw std::runtime_error("Invalid tre file version");
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
            input_stream_.seekg(offset, std::ios_base::beg);
            input_stream_.read(buffer, uncompressed_size);
        }
    }
    else if (compression == 2)
    {
        std::vector<char> compressed_data(compressed_size);
        
        {
            std::lock_guard<std::mutex> lg(mutex_);
            input_stream_.seekg(offset, std::ios_base::beg);
            input_stream_.read(&compressed_data[0], compressed_size);
        }

        int result = uncompress(
            reinterpret_cast<Bytef*>(buffer),
            reinterpret_cast<uLongf*>(&uncompressed_size),
            reinterpret_cast<Bytef*>(&compressed_data[0]),
            compressed_size);

        if (result != Z_OK)
        {
            throw std::runtime_error("ZLib error: " + boost::lexical_cast<std::string>(result));
        }
    }
    else
    {
        throw std::runtime_error("Unknown format");
    }
}
