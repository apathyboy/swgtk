
#include <tre/tre_archive.h>

#include <algorithm>
#include <ppl.h>

#include <tre/tre_file.h>

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
        tre_list_.at(filename) = make_shared<TreFile>(filename);
    });

    tre_filenames_ = move(index_files);
}

uint32_t TreArchive::GetResourceSize(const std::string& resource_name) const
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsResource(resource_name))
        {
            return (*iter).second->GetResourceSize(resource_name);
        }
    }

    throw std::runtime_error("Requested unknown resource " + resource_name);
}

std::vector<char> TreArchive::GetResource(const std::string& resource_name)
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsResource(resource_name))
        {
            return (*iter).second->GetResource(resource_name);
        }
    }

    throw std::runtime_error("Requested unknown resource " + resource_name);
}

std::string TreArchive::GetMd5Hash(const std::string& resource_name) const
{
    auto end = tre_list_.end();
    for (auto iter = tre_list_.begin(); iter != end; ++iter)
    {
        if ((*iter).second->ContainsResource(resource_name))
        {
            return (*iter).second->GetMd5Hash(resource_name);
        }
    }

    throw std::runtime_error("Requested unknown resource " + resource_name);
}

const vector<string>& TreArchive::GetTreFilenames() const
{
    return tre_filenames_;
}

vector<string> TreArchive::GetAvailableResources() const
{
    vector<string> resource_list;

    for_each(
        begin(tre_list_),
        end(tre_list_),
        [&resource_list] (const TreFileMap::value_type& tre_item)
    {
        auto resources = tre_item.second->GetResourceNames();
        resource_list.insert(begin(resource_list), begin(resources), end(resources));
    });

    return resource_list;
}
