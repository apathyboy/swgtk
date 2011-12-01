#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include <swgtk/toc_reader.h>

using namespace std;
using namespace swgtk::tre;

const std::string toc_file = "sku3_client.toc";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to swg directory>" << endl;
        exit(0);
    }

    string swg_directory(argv[1]);

    {
        TocReader toc_reader(swg_directory + "/" + toc_file);

		auto header = toc_reader.GetHeader();

        cout << "Loaded resource from archive:\n\n"
             << "   Name: " << swg_directory + "/" + toc_file << "\n"
			 << "	File Type: " << header.file_type << "\n"
			 << "	File Version: " << header.file_version << "\n"
			 << "	Format: " << header.format << "\n"
			 << "	Record Count: " << header.record_count << "\n"
			 << "	Info Compressed Size: " << header.info_compressed_size << "\n"
			 << "	Info Uncompressed Size: " << header.info_uncompressed_size << "\n"
			 << "	? Size: " << header.size << "\n"
			 << "	Tre Count: " << header.tre_count << "\n"
			 << "	Tre Name Size: " << header.tre_name_size << "\n" << endl;

    }

    return 0;
}
