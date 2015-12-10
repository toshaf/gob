#include <iostream>
#include "gob.hh"

using namespace std;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Which file do you want to dump?\n";
        return 1;
    }

    char *const path = argv[1];

    auto symbols = load_symbols(path);

    for (auto i = symbols.begin(); i != symbols.end(); ++i) {
        auto sym = *i;
        cout << sym.first << "@" << sym.second.addr << endl;
    }

    return 0;
}
