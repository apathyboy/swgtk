
#include <tre/tre_archive.h>

#include <algorithm>

#include <tre/detail/live_config_reader.h>

using namespace std;
using namespace tre;

const vector<char>& TreResourceHandle::GetBuffer() const
{
    return buffer_;
}

const string& TreResourceHandle::GetFilename() const
{
    return filename_;
}

const string& TreResourceHandle::GetTreFilename() const
{
    return tre_filename_;
}

uint32_t TreResourceHandle::GetFileSize() const
{
    return file_size_;
}

uint32_t TreResourceHandle::GetCompressedFileSize() const
{
    return compressed_file_size_;
}

const string& TreResourceHandle::GetMd5Hash() const
{
    return md5_hash_;
}

TreArchive::TreArchive()
{}

TreArchive::TreArchive(const string& archive_config)
{
    detail::LiveConfigReader reader(archive_config);

    auto index_files = reader.GetIndexFiles();
    BuildIndex(index_files);
}

void TreArchive::BuildIndex(vector<string> index_files)
{
    for_each(
        begin(index_files),
        end(index_files),
        [] (const std::string& filename)
    {

    });
}

shared_ptr<TreResourceHandle> TreArchive::GetResourceHandle(const string& resource_name)
{
    return make_shared<TreResourceHandle>();
}

vector<string> TreArchive::GetTreList() const
{
    return vector<string>();
}
