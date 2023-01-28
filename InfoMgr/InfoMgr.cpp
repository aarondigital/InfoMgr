#include "InfoMgr.h"
#include "DB.h"


using namespace std;
namespace fs = std::filesystem;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << " Usage: " << argv[0] << " StartPath" << endl;
        return -1;
    }

    
    auto static db = DB();

    const fs::path root_path{ argv[1] };

    vector<fs::path> files;

    vector<string> extensions = { ".jpg",".jpeg",".png" };


    // TODO:  This will do each directory sequentially so still could 
    //   be optimised further by pushing the directory read further down... 
    //   maybe not accurate
    int files_count = 0;

    cout << "Preparing file list" << endl;
    for (auto const& entry : fs::directory_iterator{ root_path }) {
        if (entry.is_directory()) {
            cout << endl << "Dir: " << entry;


            for (auto const& img : fs::directory_iterator{ entry.path() }) {
                //auto dir_name = base_path.string().substr(root_path.string().size() + 1);
                string dir_name = entry.path().string().substr(root_path.string().size() + 1);
                string file_name = img.path().filename().string();
                if (find(extensions.begin(), extensions.end(), img.path().extension()) != extensions.end()) {
                    vector<uchar> buf;
                    create_thumb_buf(img.path().string(), buf);
                    
                    auto result = db.insertAttachment(file_name, dir_name, buf);
                    cout << "Inserted " << file_name << endl;
                    files_count += result;
                }
                //cout << "FS_base_path: " << root_path << " | Path: " << dir_name << " | filename : " << file_name << endl;

            }

        }
        else {
            cout << "Skipping non-directory: " << entry << endl;
        }
    }

    cout << "Num of files: " << files_count << endl;

    db.close(); 


    return 0;
}

