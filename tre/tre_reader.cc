
#include <tre/tre_reader.h>

#include <cstring>
#include <algorithm>
#include <sstream>
#include <zlib.h>

using namespace std;
using namespace tre;

vector<TreFileInfo> TreReader::ReadIndex(const string& archive_name)
{
    TreHeader header = ReadHeader(archive_name);
    
    ifstream file_stream;

    file_stream.exceptions(ifstream::failbit | ifstream::badbit);
    file_stream.open(archive_name.c_str(), ios_base::binary);

    vector<TreFileInfo> data = ReadFileBlock(header, file_stream);

    ReadFileNames(header, data, file_stream);
    ReadMd5Sums(header, data, file_stream);

    return data;
}

TreHeader TreReader::ReadHeader(const string& archive_name)
{
    ifstream file_stream;

    file_stream.exceptions(ifstream::failbit | ifstream::badbit);
    file_stream.open(archive_name.c_str(), ios_base::binary);

    ValidateFileType(ReadFileType(file_stream));
    ValidateFileVersion(ReadFileVersion(file_stream));

    TreHeader header;

    header.name = archive_name;

    file_stream.read(
        reinterpret_cast<char*>(&header.record_count), 
        sizeof(header.record_count));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_info.offset), 
        sizeof(header.file_info.offset));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_info.compression), 
        sizeof(header.file_info.compression));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_info.compressed_size), 
        sizeof(header.file_info.compressed_size));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_name.compression), 
        sizeof(header.file_name.compression));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_name.compressed_size), 
        sizeof(header.file_name.compressed_size));

    file_stream.read(
        reinterpret_cast<char*>(&header.file_name.size), 
        sizeof(header.file_name.size));
    
    file_stream.close();

    header.file_info.size = header.record_count * (sizeof(uint32_t) * 6); // 6 values read.
    header.file_name.offset = header.file_info.offset + header.file_info.compressed_size;

    return header;
}

std::string TreReader::ReadFileType(ifstream& file_stream) const
{    
    std::string file_type;

    file_stream.width(4);
    file_stream >> file_type;

    return file_type;
}

std::string TreReader::ReadFileVersion(ifstream& file_stream) const
{
    std::string version;

    file_stream.width(4);
    file_stream >> version;

    return version;
}

vector<TreFileInfo> TreReader::ReadFileBlock(TreHeader header, ifstream& file_stream) const
{
    vector<char> data = header.file_info.ReadDataBlock(&file_stream);

    vector<TreFileInfo> files;

    for (uint32_t i = 0, offset = 0; i < header.record_count; ++i)
    {
        TreFileInfo file_info;
        
        memcpy( &file_info.checksum, &data[0]+offset, sizeof(file_info.checksum));
        offset += sizeof(file_info.checksum );

        memcpy( &file_info.file_data.size, &data[0]+offset, sizeof(file_info.file_data.size));
        offset += sizeof(file_info.file_data.size );

        memcpy( &file_info.file_data.offset, &data[0]+offset, sizeof(file_info.file_data.offset));
        offset += sizeof(file_info.file_data.offset );

        memcpy( &file_info.file_data.compression, &data[0]+offset, sizeof(file_info.file_data.compression));
        offset += sizeof(file_info.file_data.compression );

        memcpy( &file_info.file_data.compressed_size, &data[0]+offset, sizeof(file_info.file_data.compressed_size));
        offset += sizeof(file_info.file_data.compressed_size );

        memcpy( &file_info.name_offset, &data[0]+offset, sizeof(file_info.name_offset));
        offset += sizeof(file_info.name_offset );

        files.push_back(move(file_info));
    }

    return files;
}

void TreReader::ReadFileNames(TreHeader header, vector<TreFileInfo>& files, ifstream& file_stream) const
{
    vector<char> data = header.file_name.ReadDataBlock(&file_stream);

    for_each(
        begin(files),
        end(files),
        [&data] (TreFileInfo& file_info)
    {
        uint32_t offset = file_info.name_offset;
        file_info.filename = string(&data[file_info.name_offset]);
    });
}

void TreReader::ReadMd5Sums(TreHeader header, vector<TreFileInfo>& files, ifstream& file_stream) const
{
    file_stream.seekg(header.file_name.offset + header.file_name.compressed_size, ios_base::beg);
    
    for_each(
        begin(files),
        end(files),
        [&file_stream] (TreFileInfo& file_info)
    {
        vector<char> sum(16);
        file_stream.read(&sum[0], 16);

        file_info.md5sum = string(begin(sum), end(sum));
    });
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
