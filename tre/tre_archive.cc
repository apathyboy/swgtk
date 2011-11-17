
#include <tre/tre_archive.h>

#include <algorithm>
#include <sstream>
#include <iomanip>

#include <ppl.h>

#include <tre/detail/live_config_reader.h>

using namespace std;
using namespace tre;

TreResourceHandle::TreResourceHandle(const string& filename, TreReader* tre_reader)
    : filename_(filename)
    , tre_reader_(tre_reader)
{
    file_info_ = tre_reader_->GetFileInfo(filename);
    buffer_ = tre_reader_->GetFileData(file_info_);
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
    return file_info_.data_size;
}

string TreResourceHandle::GetMd5Hash() const
{
    string hash = tre_reader_->GetMd5Hash(file_info_);
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
        index_files.rbegin(),
        index_files.rend(),
        [this] (const std::string& filename)
    {
        unique_ptr<TreReader> reader(new TreReader(filename));

        //for_each(
        //    begin(file_index),
        //    end(file_index),
        //    [this, &reader] (std::string& filename)
        //{
        //    tre_index_.insert(make_pair(move(filename), reader.get()));
        //});
        
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

    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter)->ContainsFile(resource_name))
        {
            auto resource_handle = make_shared<TreResourceHandle>(resource_name, (*iter).get());

            resource_handles_.insert(make_pair(resource_name, resource_handle));

            return resource_handle;
        }
    }

    throw std::runtime_error("Requested unknown resource " + resource_name);
}

vector<string> TreArchive::GetTreList() const
{
    vector<string> tre_list;

    for_each(
        begin(tre_list_),
        end(tre_list_),
        [&tre_list] (const std::unique_ptr<TreReader>& reader)
    {
        tre_list.push_back(reader->GetFilename());
    });

    return tre_list;
}

vector<string> TreArchive::ListAvailableResources() const
{
    vector<string> resource_list;

    for_each(
        begin(tre_index_),
        end(tre_index_),
        [&resource_list] (const TreIndex::value_type& item)
    {
        resource_list.push_back(item.first);
    });

    return resource_list;
}
