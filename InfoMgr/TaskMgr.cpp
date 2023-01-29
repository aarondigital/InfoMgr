#include "TaskMgr.h"



using namespace std;
namespace fs = std::filesystem;


int main(int argc, char** argv)
{

    auto static db = DB();

    // Basic, if no args provided, get list of all, otherwise if an arg is provided, print that note
    if (argc == 1)
    {
        auto notes = db.getNotes();

        for (auto& e : notes) {
            string id_str = to_string(e.note_id);
            id_str.insert(id_str.begin(), 10 - id_str.length(), ' ');
            cout << id_str << " :: " << e.note_name << " (size = " << size(e.note_text) << ")" << endl;
        }
    }
    else {
        if (argc == 2) {
            try {
                int note_id = stoi(argv[1]);
                auto n = db.getNote(note_id);
                cout << "-------    " << n.note_name << "    -------" << endl;
                cout << n.note_text << endl;
            }
            catch (exception ex) {
                cout << "Invalid input: " << argv[1] << endl << ex.what() << endl;
            }
                       
        }
    }
    db.close();
    
}