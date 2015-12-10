#include <string>
#include "gob_internal.hh"

using std::string;

namespace gob {

Gob::Gob(string path)
    : path(path)
    , symbols(::load_symbols(path))
    , breakpoints()
{}

void Gob::set_breakpoint(string sym_name) {
    auto si = symbols.find(sym_name);
    if (si == symbols.end())
        throw gob_exception(string("Can't set BP, no symbol called ") + sym_name);

    auto sym = si->second;
    breakpoints.insert(std::make_pair(sym.addr, sym));
}

void Gob::run(std::function<void(Symbol)> handler) {
    ::gob_trace(path, breakpoints, handler);
}

} // gob

