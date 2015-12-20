#include <iostream>
#include <string>
#include <memory>
#include <fstream>

#include "gob.hh"

using namespace std;
using namespace gob;

#define GOB_VERSION 1.0

struct State {
    // return the next State for the machine
    // or nullptr to signify that you're done
    virtual unique_ptr<State> Run() = 0;
};

bool starts_with(string s, string prefix) {
    return !s.compare(0, prefix.size(), prefix);
}

std::string rtrim(std::string str) {
    str.erase(str.find_last_not_of(" \t") + 1);
    return str;
}

std::string ltrim(std::string str) {
    str.erase(0, str.find_first_not_of(" \t"));
    return str;
}

std::string trim(std::string str) {
    return ltrim(rtrim(str));
}

std::string get_user_input(std::istream& in, std::ostream& out) {
    string input;
    while (input.size() == 0) {
        input = "";
        out << "8= ";
        getline(in, input);
    }

    return trim(input);
}

void say_wut(std::ostream& out) {
    out << "wut??" << endl;
}

struct Load : public State {
    Load(string path)
        : path(path)
        , gob(path)
    { }
    void help(std::ostream& out) {
        out << "File loaded: " << path << endl;
        out << "Options:" << endl;
        out << "- sect:\n\tList all sections in the target" << endl;
        out << "- sym <prefix>:\n\tList symbols starting with prefix" << endl;
        out << "- br <sym>:\n\tSet breakpoint by name" << endl;
        out << "- go:\n\tRun the program" << endl;
        out << "- exit:\n\tExit GOB" << endl;
    }
    virtual unique_ptr<State> Run() {
        while (1) {
            string input = get_user_input(cin, cout);

            if (input == "help") {
                help(cout);
                continue;
            }

            if (input == "sect") {
                gob.dump_sections(cout);
                continue;
            }

            if (starts_with(input, "sym")) {
                auto prefix = trim(input.substr(3));
                auto symbols = gob.get_symbols();

                for (auto i = symbols.begin(); i != symbols.end(); ++i) {
                    auto sym = *i;
                    if (starts_with(sym.first, prefix)) {
                        cout << sym.first << "@" << sym.second.addr << endl;
                    }
                }

                continue;
            }

            if (starts_with(input, "br")) {
                auto sym = trim(input.substr(2));
                try {
                    gob.set_breakpoint(sym);
                }
                catch (gob_exception& ex) {
                    cout << ex.what() << endl;
                }

                continue;
            }

            if (input == "go") {
                gob.run([&](Symbol sym){
                        
                    auto info = gob.get_source_info(sym);
                    std::ifstream file(info.filename);
                    unsigned int counter = 0;
                    for (std::string line; std::getline(file, line) && counter < info.lineno + 3; ++counter) {
                        if (counter >= info.lineno - 1) {
                            cout << line << endl;
                        }
                    }

                    cin.ignore();
                });

                continue;
            }

            if (input == "exit") {
                return unique_ptr<State>();
            }

            say_wut(cout);
        }
    }
private:
    string path;
    Gob gob;
};

struct Init : public State {
    void help(std::ostream& out) {
        out << "No file loaded" << endl;
        out << "Options:" << endl;
        out << "- load <fname>:\n\tLoad a target program" << endl;
        out << "- exit:\n\tExit GOB" << endl;
    }
    virtual unique_ptr<State> Run() {
        while(1) {
            string input = get_user_input(cin, cout);

            if (input == "help") {
                help(cout);
                continue;
            }

            if (starts_with(input, "load")) {
                if (input.size() > 4) {
                    string path = trim(input.substr(4));

                    try {
                        return unique_ptr<State>(new Load(path));
                    } catch (gob_exception& ex) {
                        cout << ex.what() << endl;
                        continue;
                    }
                }
            }

            if (input == "exit") {
                return unique_ptr<State>();
            }

            say_wut(cout);
        }
    }
};

void run_state(std::unique_ptr<State> state) {
    do {
        state = state->Run();
    } while (state.get() != nullptr);
}

void gob_banner(std::ostream& out) {
    out << "GO deBugger v" << GOB_VERSION << endl;
    out << "^^---^--------" << endl;
    out << "For usage type 'help' at any time" << endl;
    out << endl;
}

int go(int argc, char** argv) {
    gob_banner(cout);

    unique_ptr<State> state(new Init());
    if (argc > 1) {
        state = unique_ptr<State>(new Load(argv[1]));
    }

    run_state(std::move(state));

    return 0;
}

int main(int argc, char** argv) {
    try {
        return go(argc, argv);
    }
    catch(gob_exception e) {
        cout << "BORK!" << endl;
        cout << e.what() << endl;
        return -1;
    }
}
