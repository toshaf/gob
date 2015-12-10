#include <iostream>
#include "gob.hh"

using namespace std;
using namespace gob;

int go(int argc, char** argv) {
    if (argc < 2) {
        cout << "Which file do you want to dump?\n";
        return 1;
    }

    char const* path = argv[1];

    Gob g(path);

    if (argc > 2) {
        for (auto i = 2; i < argc; ++i) {
            g.set_breakpoint(argv[i]);
        }

        g.run([=](Symbol sym){
            cout << "BP @ " << sym.name << endl;
            cin.ignore();
        });
    } else {
        auto symbols = g.get_symbols();

        for (auto i = symbols.begin(); i != symbols.end(); ++i) {
            auto sym = *i;
            cout << sym.first << "@" << sym.second.addr << endl;
        }
    }

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
