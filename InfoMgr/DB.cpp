#include "DB.h"


DB::DB()
{
    conn_string = "dbname=km user=km-cli password=fsTtKm3HR8eEzLu4rzRq hostaddr=172.16.100.6 port=5432";
    
    conn = make_unique<pg::connection>(conn_string);

    cout << "Connection to DB successful: " << conn->dbname() << endl;
    
    if (not conn->is_open()) {
        throw 2;
    }

    // TODO: abstract these
    conn->prepare("attachment_insert",
        "INSERT INTO org.attachments "
        "(filename, virtual_file_path, filesystem_id, filesystem_path, filesystem_filename, preview_blob) "
        "VALUES ($1, $2, 3, $2, $1, $3)");
        
}

Note DB::getNote(int id)
{
    
    return Note();
}

int DB::insertAttachment(string file_name, string dir_name, vector<uchar> buf) {

    
    try {
        pg::work trx(*conn);

        auto preview = pg::binary_cast(buf.data(), buf.size());

        auto result = trx.exec_prepared0("attachment_insert", file_name, dir_name, preview);

        trx.commit();

        return 1;
    }
    catch (pg::sql_error e) {
        cout << "Failed to insert " << file_name << e.what() << endl;
    }
        
}

void DB::close()
{
    conn->close();
}
