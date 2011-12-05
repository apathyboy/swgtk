#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include <swgtk/toc_reader.h>

using namespace std;
using namespace swgtk;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to toc file>" << endl;
        exit(0);
    }

    string toc_file(argv[1]);

    {
        TocReader toc_reader(toc_file);

		auto header = toc_reader.GetHeader();

        cout << "Loaded resource from archive:\n\n"
             << "   Name: " << toc_file << "\n"
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
