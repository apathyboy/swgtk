
#include <iostream>
#include <memory>
#include <string>
#include <tre/tre.h>

using namespace std;
using namespace tre;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to tre directory>" << endl;
        exit(0);
    }

    string tre_directory(argv[1]);

    {
        string filename = tre_directory + "/patch_01.tre";
        Tre tre_file(filename);

        cout << "Reading tre file: " << filename << "\n\n";
        cout << "   Version: " << tre_file.GetVersion() << endl;
    }

    return 0;        
}
