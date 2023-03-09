#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>

using namespace std;

//Turns the program file into a string for easier management
//Also gets rid of comments and spaces
string programToString(string &filename) {
    ifstream fin;
    fin.open(filename, ios::in);
    if (fin.fail()) {
        cout << "Couldn't open " << filename << endl;
        return "";
    }
    string bfProgram;
    char a;
    while (fin >> skipws >> a) {
        if (a == '+' || a == '-' || a == '>' || a == '<' || a == '[' || a == ']' || a == '.' || a == ',') {
            bfProgram += a;
        }
    }
    fin.close();
    return bfProgram;
}

string getFileExtension(string &filename) {
    return filename.substr(filename.find_last_of('.') + 1);
}

int main(int argc, char *argv[]) {
    //Startup
    string filename;
    if (argc > 1 && strcmp(argv[1], "--input-prompt") != 0) {
        filename = argv[1];
    } else {
        cout << "Brainfuck program: ";
        getline(cin, filename);
    }
    bool inputPrompt = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--input-prompt") == 0) {
            inputPrompt = true;
        }
    }

    //Setup
    if (getFileExtension(filename).compare("bf") != 0) {
        cout << "The file extension isn't .bf but will try to run anyways." << endl;
    }
    string program = programToString(filename);
    vector<unsigned char> memory(30000, 0);
    int pointer = 0;
    int instructionIndex = 0;

    //Interpreter
    while (instructionIndex < program.size()) {
        char instruction = program[instructionIndex];
        if (instruction == '+') {
            memory[pointer]++;
        } else if (instruction == '-') {
            memory[pointer]--;
        } else if (instruction == '>') {
            pointer = (pointer == 29999) ? 0 : pointer+1;
        } else if (instruction == '<') {
            pointer = (pointer == 0) ? 29999 : pointer-1;
        } else if (instruction == '.') {
            cout << memory[pointer];
        } else if (instruction == ',') {
            if (inputPrompt) {
                cout << endl << "Input one character: ";
            }
            string s;
            getline(cin, s);
            memory[pointer] = (s.empty()) ? 0 : s[0];
        } else if (instruction == '[') {
            if (memory[pointer] == 0) {
                int openBrackets = 1;
                for (int i = instructionIndex+1; i < program.size() && openBrackets > 0; i++) {
                    if (program[i] == '[') {
                        openBrackets++;
                    } else if (program[i] == ']') {
                        openBrackets--;
                        if (openBrackets == 0) {
                            instructionIndex = i;
                        }
                    }
                }
                if (openBrackets > 0) {
                    cout << endl << "There's an opening bracket that doesn't close on instruction " << instructionIndex << endl;
                    return 1;
                }
            }
        } else if (instruction == ']') {
            int closingBrackets = 1;
            for (int i = instructionIndex-1; i >= 0 && closingBrackets > 0; i--) {
                if (program[i] == ']') {
                    closingBrackets++;
                } else if (program[i] == '[') {
                    closingBrackets--;
                    if (closingBrackets == 0) {
                        instructionIndex = i - 1;
                    }
                }
            }
            if (closingBrackets > 0) {
                cout << endl << "There's a closing bracket that doesn't open on instruction " << instructionIndex << endl;
                return 1;
            }
        }
        instructionIndex++;
    }
    return 0;
}