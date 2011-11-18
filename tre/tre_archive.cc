
#include <tre/tre_archive.h>

#include <algorithm>
#include <ppl.h>

using namespace std;
using namespace tre;
using namespace Concurrency;

TreArchive::TreArchive(vector<string> index_files)
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

uint32_t TreArchive::GetFilesize(const std::string& filename) const
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsFile(filename))
        {
            return (*iter).second->GetFileSize(filename);
        }
    }

    throw std::runtime_error("Requested unknown resource " + filename);
}

std::vector<char> TreArchive::GetFileData(const std::string& filename)
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsFile(filename))
        {
            return (*iter).second->GetFileData(filename);
        }
    }

    throw std::runtime_error("Requested unknown resource " + filename);
}

std::string TreArchive::GetMd5Hash(const std::string& filename) const
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsFile(filename))
        {
            return (*iter).second->GetMd5Hash(filename);
        }
    }

    throw std::runtime_error("Requested unknown resource " + filename);
}

const vector<string>& TreArchive::GetTreFilenames() const
{
    return tre_filenames_;
}

vector<string> TreArchive::GetAvailableFiles() const
{
    vector<string> resource_list;
    for_each(
        begin(tre_list_),
        end(tre_list_),
        [&resource_list] (const TreReaderMap::value_type& tre_item)
    {
       // auto files = tre_item.second->GetFilenames();

       // resource_list.insert(begin(files), end(files));
    });

    return resource_list;
}
