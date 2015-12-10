#include <bfd.h>
#include <map>
#include <string>
#include <exception>
#include <vector>
#include "gob.hh"

using std::map;
using std::string;
using std::exception;
using std::vector;

map<string, int> load_symbols(char const* path) {
    
    bfd_init();

    bfd *abfd = bfd_openr(path, NULL);
    auto cleanup = [=](){bfd_close(abfd);};
    defer<decltype(cleanup)> d(cleanup);

    if (!abfd)
        throw gob_exception("Couldn't load file");

    if(!bfd_check_format(abfd, bfd_object))
        throw gob_exception("Not a valid object file");

    int symtab_size = bfd_get_symtab_upper_bound(abfd);

    vector<asymbol*> asymtab(symtab_size);
    int numsymbols = bfd_canonicalize_symtab(abfd, asymtab.data());

    map<string, int> symbols;

    for (int i=0; i<numsymbols; ++i) {
        string name(bfd_asymbol_name(asymtab[i]));
        int value = bfd_asymbol_value(asymtab[i]);

        symbols.insert(std::pair<string, int>(name, value));
    }

    return symbols;
}
