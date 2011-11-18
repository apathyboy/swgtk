#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <tre/tre_reader.h>

using namespace std;
using namespace tre;

const std::string resource_name = "appearance/mesh/rock_formation_a1_l0.msh";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to swg directory>" << endl;
        exit(0);
    }

    string swg_directory(argv[1]);

    {
        TreReader reader(swg_directory + "/patch_00.tre");
        
        cout << "Loaded resource from archive:\n\n"
             << "   Name: " << swg_directory + "/bottom.tre" << "\n"
             << "   File Count: " << reader.GetFileCount() << "\n" << endl;
        
        cout << "Finished indexing\n" << endl;
    }

    return 0;
}
