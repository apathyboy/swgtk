
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <tre/tre_archive.h>

using namespace std;
using namespace tre;

const std::string resource_name = "appearance/mesh/ply_corl_house_lg_s01_fp1_r0_mesh_r0_l2.msh";

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <path to swg directory>" << endl;
        exit(0);
    }
    
    auto start_time = chrono::high_resolution_clock::now();

    string swg_directory(argv[1]);

    {
        TreArchive archive;
        
        vector<string> index_files;

        index_files.push_back(swg_directory + "/bottom.tre");
        index_files.push_back(swg_directory + "/data_animation_00.tre");
        index_files.push_back(swg_directory + "/data_music_00.tre");
        index_files.push_back(swg_directory + "/data_other_00.tre");
        index_files.push_back(swg_directory + "/data_sample_00.tre");
        index_files.push_back(swg_directory + "/data_sample_01.tre");
        index_files.push_back(swg_directory + "/data_sample_02.tre");
        index_files.push_back(swg_directory + "/data_sample_03.tre");
        index_files.push_back(swg_directory + "/data_sample_04.tre");
        index_files.push_back(swg_directory + "/data_skeletal_mesh_00.tre");
        index_files.push_back(swg_directory + "/data_skeletal_mesh_01.tre");
        index_files.push_back(swg_directory + "/data_sku1_00.tre");
        index_files.push_back(swg_directory + "/data_sku1_01.tre");
        index_files.push_back(swg_directory + "/data_sku1_02.tre");
        index_files.push_back(swg_directory + "/data_sku1_03.tre");
        index_files.push_back(swg_directory + "/data_sku1_04.tre");
        index_files.push_back(swg_directory + "/data_sku1_05.tre");
        index_files.push_back(swg_directory + "/data_sku1_06.tre");
        index_files.push_back(swg_directory + "/data_sku1_07.tre");
        index_files.push_back(swg_directory + "/data_static_mesh_00.tre");
        index_files.push_back(swg_directory + "/data_static_mesh_01.tre");
        index_files.push_back(swg_directory + "/data_texture_00.tre");
        index_files.push_back(swg_directory + "/data_texture_01.tre");
        index_files.push_back(swg_directory + "/data_texture_02.tre");
        index_files.push_back(swg_directory + "/data_texture_03.tre");
        index_files.push_back(swg_directory + "/data_texture_04.tre");
        index_files.push_back(swg_directory + "/data_texture_05.tre");
        index_files.push_back(swg_directory + "/data_texture_06.tre");
        index_files.push_back(swg_directory + "/data_texture_07.tre");
        index_files.push_back(swg_directory + "/default_patch.tre");
        index_files.push_back(swg_directory + "/patch_01.tre");
        index_files.push_back(swg_directory + "/patch_02.tre");
        index_files.push_back(swg_directory + "/patch_03.tre");
        index_files.push_back(swg_directory + "/patch_04.tre");
        index_files.push_back(swg_directory + "/patch_05.tre");
        index_files.push_back(swg_directory + "/patch_06.tre");
        index_files.push_back(swg_directory + "/patch_07.tre");
        index_files.push_back(swg_directory + "/patch_08.tre");
        index_files.push_back(swg_directory + "/patch_09.tre");
        index_files.push_back(swg_directory + "/patch_10.tre");
        index_files.push_back(swg_directory + "/patch_11_00.tre");
        index_files.push_back(swg_directory + "/patch_11_01.tre");
        index_files.push_back(swg_directory + "/patch_11_02.tre");
        index_files.push_back(swg_directory + "/patch_11_03.tre");
        index_files.push_back(swg_directory + "/patch_12_00.tre");
        index_files.push_back(swg_directory + "/patch_13_00.tre");
        index_files.push_back(swg_directory + "/patch_14_00.tre");
        
        archive.BuildIndex(move(index_files));
        
        auto files = archive.GetTreList();
        
        cout << "Loading resources from the following files:\n\n";
        copy(begin(files), end(files), ostream_iterator<string>(cout, "\n")); 

        auto handle = archive.GetResourceHandle(resource_name);
        
        cout << "\nLoaded resource from archive:\n\n"
             << "   Name: " << handle->GetFilename() << "\n"
             << "   Filesize: " << handle->GetFileSize() << "\n"
             << "   MD5: " << handle->GetMd5Hash() << "\n\n"
             << endl;
    }

    auto stop_time = chrono::high_resolution_clock::now();

    cout << "Duration: " << chrono::duration_cast<chrono::milliseconds>(stop_time - start_time).count() << "ms" << endl;

    return 0;
}
