
#include <tre/tre_archive.h>

#include <algorithm>

#include <tre/detail/live_config_reader.h>

using namespace std;
using namespace tre;

TreResourceHandle::TreResourceHandle(const TreResourceFile& tre_info)
    : file_info_(tre_info)
{}

const vector<char>& TreResourceHandle::GetBuffer() const
{
    return buffer_;
}

const string& TreResourceHandle::GetFilename() const
{
    return file_info_.filename;
}

uint32_t TreResourceHandle::GetFileSize() const
{
    return file_info_.info.data_size;
}

const string& TreResourceHandle::GetMd5Hash() const
{
    return file_info_.md5sum;
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
        [this] (const std::string& filename)
    {
        unique_ptr<TreReader> reader(new TreReader(filename));
        auto file_index = reader->ReadIndex();

        tre_index_.insert(begin(file_index), end(file_index));
        tre_list_.push_back(move(reader));
    });
}

shared_ptr<TreResourceHandle> TreArchive::GetResourceHandle(const string& resource_name)
{
    auto find_resource_iter = resource_handles_.find(resource_name);
    if (find_resource_iter != resource_handles_.end())
    {
        return find_resource_iter->second;
    }

    auto find_file_info_iter = tre_index_.find(resource_name);

    if (find_file_info_iter == tre_index_.end())
    {
        throw std::runtime_error("Requested unknown resource " + resource_name);
    }

    auto resource_handle = make_shared<TreResourceHandle>(find_file_info_iter->second);

    resource_handles_.insert(make_pair(resource_name, resource_handle));

    return resource_handle;
}

vector<string> TreArchive::GetTreList() const
{
    vector<string> tre_list;

    for_each(
        begin(tre_list_),
        end(tre_list_),
        [&tre_list] (const std::unique_ptr<TreReader>& item)
    {
        tre_list.push_back(item->GetFilename());
    });

    return tre_list;
}

vector<string> TreArchive::ListAvailableResources() const
{
    vector<string> resource_list;

    for_each(
        begin(tre_index_),
        end(tre_index_),
        [&resource_list] (const TreContentsMap::value_type& item)
    {
        resource_list.push_back(item.first);
    });

    return resource_list;
}
