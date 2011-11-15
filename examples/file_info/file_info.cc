
#include <iostream>
#include <memory>
#include <string>
#include <tre/tre_archive.h>

using namespace std;
using namespace tre;

const std::string resource_name = "appearance/mesh/rock_formation_a1_l0.msh";

void PrintHexView(const shared_ptr<TreResourceHandle>& handle);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to swg directory>" << endl;
        exit(0);
    }

    string swg_directory(argv[1]);

    {
        TreArchive archive;

        vector<string> index_files;
        index_files.push_back(swg_directory + "/patch_00.tre");

        archive.BuildIndex(index_files);

        auto files = archive.GetTreList();

        cout << "Loading resources from the following files:\n\n";
        copy(begin(files), end(files), ostream_iterator<string>(cout, "\n")); 

        //auto handle = archive.GetResourceHandle(resource_name);
        //
        //cout << "\nLoaded resource from archive:\n\n"
        //     << "   Name: " << handle->GetFilename() << "\n"
        //     << "   Tre: " << handle->GetTreFilename() << "\n"
        //     << "   Filesize: " << handle->GetFileSize() << "\n"
        //     << "   Filesize (compressed): " << handle->GetCompressedFileSize() << "\n"
        //     << "   MD5: " << handle->GetMd5Hash() << "\n\n"
        //     << endl;
        //
        //PrintHexView(handle);
    }

    return 0;
}


void PrintHexView(const shared_ptr<TreResourceHandle>& handle)
{}
