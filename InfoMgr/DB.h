#pragma once

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <opencv2/core/types_c.h> // TODO: should really pass better stuff around

using namespace std;
namespace pg = pqxx;

class Note {
public:
    int note_id;
    string note_name;
    vector<string> note_category;
    string note_text;
    string note_text_type;
    Note(int, string, string);
    Note();
};

class Task {
public:
    int todo_id;
    string todo_name;
    string todo_text;
};

class DB {
public:
    DB();
    Note getNote(int id);
    int insertAttachment(string file_name, string dir_name, vector<uchar> buf);
    vector<Note> getNotes();
    void close();

private:
    string conn_string;
    unique_ptr<pg::connection> conn;
};