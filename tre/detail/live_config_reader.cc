
#include <tre/detail/live_config_reader.h>

using namespace std;
using namespace tre::detail;

LiveConfigReader::LiveConfigReader(const std::string& live_file)
{}

const string& LiveConfigReader::GetIndexType() const
{
    return index_type_;
}

const vector<string>& LiveConfigReader::GetIndexFiles() const
{
    return index_files_;
}
