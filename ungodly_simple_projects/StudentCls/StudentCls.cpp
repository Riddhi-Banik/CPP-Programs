// 100% handwritten(concepts assisted by AI); but still dont ask questions about my code...

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <map>
#include <format>
#include <functional>
#include <algorithm>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

// paragraphs: # btw i formatted in this such for readibility and runtime flexibility
const string helpCmds[] = {
    "Here are all the commands and their functionalities: \n",
    "[help]   : Gives you the command palette",
    "[insert] : Gives you to insert Student data (Name, Class, Section, Roll)",
    "[view]   : Gives you the entire list of the Student database",
    "[update] : Gives you to update a student targetted by its ",
    "[delete] : Gives you to delete a student by its ID",
    "[save]   : Saves the DataBase to a file."
};
const map<string, function<string(vector<string>)>> ContextTemplate = {
    {"id_name_TMPLT", [](const vector<string> &Data){return "["+Data[0]+"] : ("+Data[1]+")";}},
    {"student_fullData_TMPLT", [](const vector<string> &Data){return "__________\nName: "+Data[0]+"\nClass: "+Data[1]+"\nSec: "+Data[2]+"\nRoll: "+Data[3]+"\n";}},
    {"student_insert_success_TMPLT", [](const vector<string> &Data){return "Student Added Successfully: "+Data[0]+" ("+Data[1]+", "+Data[2]+", "+Data[3]+")";}},
    {"student_update_success_TMPLT", [](const vector<string> &Data){return "Student Updated Successfully: "+Data[0]+" ("+Data[1]+", "+Data[2]+", "+Data[3]+")";}}
};
// functions
string capitalize(string str) { // ill admit this is copy pasted
    if (str.empty()) return "";
    
    str[0] = static_cast<char>(toupper(static_cast<unsigned char>(str[0])));
    
    transform(str.begin() + 1, str.end(), str.begin() + 1, [](unsigned char c) { 
        return tolower(c); 
    });

    return str;
}
unsigned int asciifier(string word) {
    unsigned int res = 0;

    reverse(word.begin(), word.end());;

    unsigned int counter = 0;unsigned int conbase = 1;

    for (char c : word){if(int(c) == 32){continue;}
        conbase *= 26;
        int cnt = static_cast<int>(c) - 64;
        res = (res + (cnt)*(conbase*(++counter)));
    }
    return (res/conbase);
};
// pseudo pythonic functions
void print(const string &p, const string &end = "\n"){
    cout << p << end;
};
string input(const string &str) {
    string var;

    cout << str;
    getline(cin, var);

    return var;
};

vector<string> split(string s, char splt = ' '){ // 
    if (s.find(splt) == string::npos) {return {s};};
    vector<string> res; int pos = 0;
    do {
        pos = s.find(splt);
        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1);

    } while (s.find(splt) != string::npos);
    res.push_back(s);
    return res;
};

// C++ Data boilerplates

struct studentPos {
    int Class_;
    char Sec_;
    int Roll_;
};

class Student {
    private:
        unsigned int ID;
        static int counter;
    public:
        unsigned int ID_gen(const string &name, int cls, char sec, int roll){
            return 1 + ((cls - 1) + (static_cast<int>(sec)-65)*10 + (roll - 1)*10*26)*asciifier(name);
        }
        string name_;
        studentPos Pos_;
    Student(string name = "", int cls = 0, char sec = 'n', int roll = 0) {
        
        if (sec >= 'a' && sec <= 'z') {sec -= 32;}

        name_ = name;
        Pos_.Class_ = cls; // 10 base
        Pos_.Sec_ = sec; // 26 base (ascii - 64)
        Pos_.Roll_ = roll; // not set

        
        // since the base of roll is unset, we will do as base: (roll)(sec)(cls)
        ID = ID_gen(name, cls, sec, roll);
    }

    unsigned int get_ID(){return ID;}

    friend void to_json(json &j, const Student &s){
        j = json{
            {"name", s.name_},
            {"cls", to_string(s.Pos_.Class_)},
            {"sec", string(1, s.Pos_.Sec_)},
            {"roll", to_string(s.Pos_.Roll_)}
        };
    }
    friend void from_json(const json &j, Student &s) {
        s.name_ = j.at("name").get<string>();
        s.Pos_.Class_ = stoi(j.at("cls").get<string>());
        s.Pos_.Sec_ = j.at("sec").get<string>()[0];
        s.Pos_.Roll_ = stoi(j.at("roll").get<string>());
        s.ID = s.ID_gen(s.name_, s.Pos_.Class_, s.Pos_.Sec_, s.Pos_.Roll_);
    }

};


map<string, int> commandMap = {
    {"exit",  -1},
    {"help",   0},
    {"view",   1},
    {"insert", 2},
    {"update", 3},
    {"delete", 4},
    {"save",   5},

};

string filename = "database.txt";

int main() {
    int banger = 3;
    print("Welcome to Student Manager: you may type \"help\" to get the manual");
    ifstream DB_FILE;
    DB_FILE.open(filename);
    json RAM; 
    
    if (DB_FILE.is_open()){
        
        cout<<"Opened file: (" + filename + ") successfully.\n";

        if (DB_FILE.peek() == ifstream::traits_type::eof())
            RAM = json::object();
        else
            RAM = json::parse(DB_FILE);
    } else {
        RAM = json::object();
    }
    

    bool do_exit = false;
    while (!do_exit) {
        
        string V_input;
        V_input = input("\n/> ");

        // get the first cmdSeg..

        vector<string> inputSeg = split(V_input);
        if (inputSeg.empty() || inputSeg[0].empty()) {
            continue;
        }

        int commandNum = commandMap[inputSeg[0]];

        if (commandNum == 0 && inputSeg[0] != "help"){
            banger++;
            if (banger == 4){
                print("Looks like you don\'t know about commands... showing help menu");
                for (string line : helpCmds) 
                    {print(line);};
                
                banger = 0;
            }
            continue;
        }
        
        switch (commandNum) { // ________________________________________
            case -1: // exit
                do_exit = true;
                break;
            case 0: // help

                for (string line : helpCmds) 
                    {print(line);};
                break;
            case 1: // view
            {
                if (RAM.empty()){
                    print("\nMemory doesn\'t have anything in here yet...");continue;
                }
                if (inputSeg.size() == 1) 
                { // get all students name and ID
                    for(const auto &[key, value]: RAM.items()){ // pseudo structure: [ID: studentObj]
                        cout << ContextTemplate.at("id_name_TMPLT")(vector<string>{key, (value["name"].get<string>())}) << "\n";
                    }; print("\nTo get more in details, add students id after view (eg.){view 1234}");

                } else if (inputSeg.size() == 2)
                { // get the student accord of its ID
                    if (!RAM.contains(inputSeg[1])){
                        print("ID doesn\'t exist");continue;
                    }
                    const json &studentData = RAM.at(inputSeg[1]);
                    
                    Student target(
                        studentData.at("name").get<string>(),
                        stoi(studentData.at("cls").get<string>()),
                        studentData.at("sec").get<string>()[0],
                        stoi(studentData.at("roll").get<string>())
                    );
                    cout << ContextTemplate.at("student_fullData_TMPLT")(
                        vector<string>{
                            target.name_,
                            to_string(target.Pos_.Class_),
                            string(1, target.Pos_.Sec_),
                            to_string(target.Pos_.Roll_),
                        } // thx copilot
                    ) << "\n";
                  
                } else 
                {
                    print("Error! invalid argument amount.[EXIT without execution]");
                }

                break;
            }
            case 2: // insert
            {
                inputSeg.at(1) = capitalize(inputSeg.at(1));
                if (inputSeg.size() == 5 && inputSeg[3].size() == 1) {
                    if (inputSeg[1].size() < 2 || inputSeg[1].size() > 27){
                        print("Error: name too short or too long (min:3;max:26)");continue;
                    }
                } else {
                    print("Error: Insertion was incorrect...");
                    continue;
                }
                Student stdnt(
                    inputSeg[1], 
                    stoi(inputSeg[2]), 
                    inputSeg[3][0], 
                    stoi(inputSeg[4])
                );
                string key = to_string(stdnt.get_ID());
                RAM[key] = stdnt;

                
               
                print(ContextTemplate.at("student_insert_success_TMPLT")({
                    inputSeg[1], 
                    inputSeg[2], 
                    inputSeg[3],
                    inputSeg[4],
                }), ""); cout << " ID: " << stdnt.get_ID() << "\n";

                break;
            }
            case 3:  // update
            {  
                inputSeg.at(2) = capitalize(inputSeg.at(2));
                const string key_array[4] = {"name", "cls", "sec", "roll"};
                if (!RAM.contains(inputSeg[1])){
                    print("ID doesn\'t exist");continue;
                }
                json StudentData = RAM.at(inputSeg[1]);
                for (int i = 0;i < 4;i++){
                    string key = key_array[i];
                    string lined_data = inputSeg[2 + i];
                    if (lined_data == "_"){continue;}
                    StudentData[key] = lined_data;
                }
                
                unsigned int new_ID = Student().ID_gen(
                    StudentData["name"].get<string>(),
                    stoi(StudentData["cls"].get<string>()),
                    StudentData["sec"].get<string>()[0],
                    stoi(StudentData["roll"].get<string>())
                );
                if (inputSeg[1] != to_string(new_ID)){
                    if (RAM.contains(to_string(new_ID))){
                        print("Error: update would create an existing student ID. No changes were made.");
                        continue;
                    }
                } else {
                    print("Tag: Nothing has changed, no changes are made.");
                    continue;
                }
                RAM.erase(inputSeg[1]);
                RAM[to_string(new_ID)] = StudentData;
                print(ContextTemplate.at("student_update_success_TMPLT")({
                    StudentData["name"], 
                    StudentData["cls"], 
                    StudentData["sec"],
                    StudentData["roll"],
                }));

                break;
            }
            case 4: // delete
            {
                if (RAM.contains(inputSeg[1])){
                    RAM.erase(inputSeg[1]);
                    print("Successfully deleted ", "");
                    cout << "("+inputSeg[1]+")\n";
                } else {
                    print("ID doenst exist, no action was made...");
                    continue;
                }
                break;
            }
            case 5: // save
            {
                ofstream DB_FILE;
                DB_FILE.open("database.txt");
                
                if (DB_FILE.is_open()){
                    DB_FILE << RAM.dump(4);
                    DB_FILE.close();
                }
                print("Saved the database successsfully to: ", ""); cout << filename << "\n";
                break;
            }
        }

       
    };

    return 0;
};
