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

//Checks that every loop in the program has an open and close bracket and stores their positions, so it doesn't have to do it every time it reaches one
vector<pair<int, int>> getLoops(string &program) {
    vector<pair<int, int>> loops;
    for (int i = 0; i < program.size(); i++) {
        if (program[i] == '[') {
            pair<int, int> loop;
            loop.first = i;
            int openBrackets = 1;
            for (int j = i+1; j < program.size() && openBrackets > 0; j++) {
                if (program[j] == '[') {
                    openBrackets++;
                } else if (program[j] == ']') {
                    openBrackets--;
                    if (openBrackets == 0) {
                        loop.second = j;
                    }
                }
            }
            if (openBrackets > 0) {
                cout << "There's a closing bracket missing";
                return {{-1, -1}};
            }
            loops.push_back(loop);
        }
    }
    return loops;
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
    if ((argc == 2 && strcmp(argv[1], "--input-prompt") == 0) || (argc == 3 && strcmp(argv[2], "--input-prompt") == 0)) {
        inputPrompt = true;
    }

    //Setup
    string program = programToString(filename);
    vector<unsigned char> memory(30000, 0);
    int pointer = 0;
    int instructionIndex = 0;
    int loopStart;
    int loopEnd;
    int currentLoop = 0;
    vector<pair<pair<int, int>, int>> pendingLoops;
    vector<pair<int, int>> loops = getLoops(program);
    if (!loops.empty() && loops[0].first == -1) {
        return 1;
    } else if (!loops.empty()) {
        loopStart = loops[currentLoop].first;
        loopEnd = loops[currentLoop].second;
        pendingLoops.push_back({loops[currentLoop], currentLoop});
    }

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
            //If we're not in the right loop, it will get it from the loops list and set it as current and add it to the pending loops list
            if (loops[currentLoop].first != instructionIndex) {
                for (int i = 0; i < loops.size(); i++) {
                    if (loops[i].first == instructionIndex) {
                        currentLoop = i;
                        loopStart = loops[i].first;
                        loopEnd = loops[i].second;
                        pendingLoops.push_back({loops[i], currentLoop});
                    }
                }
            }
            if (memory[pointer] == 0) {
                instructionIndex = loopEnd;
                pendingLoops.pop_back();
                //If there's nesting loops, it will set the parent as current
                if (!pendingLoops.empty()) {
                    loopStart = pendingLoops[pendingLoops.size()-1].first.first;
                    loopEnd = pendingLoops[pendingLoops.size()-1].first.second;
                    currentLoop = pendingLoops[pendingLoops.size()-1].second;
                }
            }
        } else if (instruction == ']') {
            instructionIndex = loopStart - 1;
        }
        instructionIndex++;
    }
    return 0;
}