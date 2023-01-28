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

Note DB::getNote(int id)
{
    pg::work trx(*conn);
    try {
        auto r = trx.exec1("SELECT note_id, note_name, note_text FROM ORG.Notes WHERE note_id = " + trx.quote(id));
        Note n(r[0].as<int>(), r[1].as<string>(), r[2].as<string>());
        return n;
    }
    catch (pg::sql_error err) {
        cout << "Error: " << err.what() << endl;
        return Note();
    }
}


vector<Note> DB::getNotes()
{
    vector<Note> notes;
    pg::work trx(*conn);
    try {
        auto res = trx.exec("SELECT note_id, note_name, note_text FROM ORG.Notes");
    
    for (pg::result::size_type i = 0; i < size(res); ++i) {
        auto r = res[i];
        int note_id = r[0].as<int>();
        string note_name = r[1].as<string>();
        string note_text = r[2].as<string>();
        Note n(note_id, note_name, note_text);
        notes.push_back(n);
    }
    return notes;
    }
    catch (pg::sql_error ex) {
        cout << "Error: " << ex.what() << endl;
    }

}

void DB::close()
{
    conn->close();
}

Note::Note(int id, string name, string text)
{
    note_id = id;
    note_name = name;
    note_text = text;
}

Note::Note()
{
    note_id = -1;
}
