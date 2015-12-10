#include <bfd.h>
#include <map>
#include <string>
#include <exception>
#include <vector>
#include "gob_internal.hh"

using std::map;
using std::string;
using std::exception;
using std::vector;

using namespace gob;

map<string, Symbol> load_symbols(string path) {
    
    bfd_init();

    bfd *abfd = bfd_openr(path.c_str(), NULL);
    auto cleanup = [=](){bfd_close(abfd);};
    defer<decltype(cleanup)> d(cleanup);

    if (!abfd)
        throw gob_exception("Couldn't load file");

    if(!bfd_check_format(abfd, bfd_object))
        throw gob_exception("Not a valid object file");

    int symtab_size = bfd_get_symtab_upper_bound(abfd);

    vector<asymbol*> asymtab(symtab_size / sizeof(asymbol*));
    int numsymbols = bfd_canonicalize_symtab(abfd, asymtab.data());

    map<string, Symbol> symbols;

    for (int i=0; i<numsymbols; ++i) {
        Symbol sym(
            bfd_asymbol_name(asymtab[i])
        ,   bfd_asymbol_value(asymtab[i])
        );
        symbols.insert(std::pair<string, Symbol>(sym.name, sym));
    }

    return symbols;
}
