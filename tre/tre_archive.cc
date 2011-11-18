
#include <tre/tre_archive.h>

#include <algorithm>
#include <sstream>
#include <iomanip>

#include <ppl.h>

#include <tre/detail/live_config_reader.h>

using namespace std;
using namespace tre;
using namespace Concurrency;

TreResourceHandle::TreResourceHandle(const string& filename, TreReader* tre_reader)
    : filename_(filename)
    , tre_reader_(tre_reader)
{
    buffer_ = tre_reader_->GetFileData(filename);
}

const vector<char>& TreResourceHandle::GetBuffer() const
{
    return buffer_;
}

string TreResourceHandle::GetFilename() const
{
    return filename_;
}

uint32_t TreResourceHandle::GetFileSize() const
{
    return tre_reader_->GetFilesize(filename_);
}

string TreResourceHandle::GetMd5Hash() const
{
    string hash = tre_reader_->GetMd5Hash(filename_);
    stringstream ss;

    ss.flags(ss.hex);
    ss.fill('0');
    ss.width(2);
    
    for_each(begin(hash), 
        end(hash),
        [&ss] (unsigned char c) 
    {
        ss << static_cast<unsigned>(c);
    });

    return ss.str();
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
        [this] (const string& filename)
    {
        tre_list_.insert(make_pair(filename, nullptr));
    });

    parallel_for_each(
        index_files.begin(),
        index_files.end(),
        [this] (const string& filename)
    {
        tre_list_.at(filename) = unique_ptr<TreReader>(new TreReader(filename));
    });

    tre_filenames_ = move(index_files);
}

shared_ptr<TreResourceHandle> TreArchive::GetResourceHandle(const string& resource_name)
{
    auto find_resource_iter = resource_handles_.find(resource_name);
    if (find_resource_iter != resource_handles_.end())
    {
        return find_resource_iter->second;
    }

    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsFile(resource_name))
        {
            auto resource_handle = make_shared<TreResourceHandle>(resource_name, (*iter).second.get());

            resource_handles_.insert(make_pair(resource_name, resource_handle));

            return resource_handle;
        }
    }

    throw std::runtime_error("Requested unknown resource " + resource_name);
}

const vector<string>& TreArchive::GetTreFilenames() const
{
    return tre_filenames_;
}

vector<string> TreArchive::ListAvailableResources() const
{
    vector<string> resource_list;
    for_each(
        begin(tre_list_),
        end(tre_list_),
        [&resource_list] (const TreReaderMap::value_type& tre_item)
    {
        //tre_item.second->GetFi;
    });

    return resource_list;
}
