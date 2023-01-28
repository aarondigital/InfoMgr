#include "InfoMgr.h"


using namespace std;
namespace pg = pqxx;
namespace fs = std::filesystem;


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << " Usage: " << argv[0] << " StartPath" << endl;
        return -1;
    }


    // TODO: Extract this from main thread
    pg::connection conn("dbname=km user=km-cli password=fsTtKm3HR8eEzLu4rzRq hostaddr=172.16.100.6 port=5432");
    if (conn.is_open()) {
        cout << "Connection to DB successful: " << conn.dbname() << endl;
    }
    else {
        cout << "DB connection failed" << endl;
        return 2;
    }



    // Prepared statement for insert
    conn.prepare("attachment_insert",
        "INSERT INTO org.attachments "
        "(filename, virtual_file_path, filesystem_id, filesystem_path, filesystem_filename, preview_blob) "
        "VALUES ($1, $2, 3, $2, $1, $3)");

    pg::work trx(conn);

    // End TODO

    const fs::path root_path{ argv[1] };

    vector<fs::path> files;

    vector<string> extensions = { ".jpg",".jpeg",".png" };


    // TODO:  This will do each directory sequentially so still could 
    //   be optimised further by pushing the directory read further down... 
    //   maybe not accurate
    cout << "Preparing file list" << endl;
    for (auto const& entry : fs::directory_iterator{ root_path }) {
        if (entry.is_directory()) {
            cout << endl << "Dir: " << entry;

            // DB operations
            // Tranaction
            //pg::work trx(conn);

            for (auto const& img : fs::directory_iterator{ entry.path() }) {
                //auto dir_name = base_path.string().substr(root_path.string().size() + 1);
                string dir_name = entry.path().string().substr(root_path.string().size() + 1);
                string file_name = img.path().filename().string();
                if (find(extensions.begin(), extensions.end(), img.path().extension()) != extensions.end()) {
                    vector<uchar> buf;
                    create_thumb_buf(img.path().string(), buf);
                    auto preview = pg::binary_cast(buf.data(), buf.size());

                    auto result = trx.exec_prepared("attachment_insert", file_name, dir_name, preview);
                }
                //cout << "FS_base_path: " << root_path << " | Path: " << dir_name << " | filename : " << file_name << endl;

            }

            trx.commit();

            // //Temp, just one directory please
            //conn.close();
            //return 0; 

            //vector<std::thread> threads;

            //for (auto const& img : fs::directory_iterator{ entry.path() }) {
            //    threads.push_back(std::thread(convert_image, img));
            //    //files.push_back(img);
            //    //convert_image(img);
            //}

            //for (auto& th : threads) {
            //    th.join();
            //}

        }
        else {
            cout << "Skipping " << entry << endl;
        }
    }

    trx.commit();
    cout << "Num of files: " << files.size() << endl;

    conn.close();

    return 0;
}

