#include <bfd.h>
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

void run_sect(bfd *abfd, asection *sect, PTR obj) {
    std::ostream& out = *reinterpret_cast<std::ostream*>(obj);

    out << sect->name << std::endl;
}

void Gob::dump_sections(std::ostream& out) const {
    bfd_init();

    bfd *abfd = bfd_openr(path.c_str(), NULL);
    if (!abfd)
        throw gob_exception("Couldn't load file");
    auto cleanup = [=](){ bfd_close(abfd); };
    defer<decltype(cleanup)> d(cleanup);

    if (!bfd_check_format(abfd, bfd_object))
        throw gob_exception("Not a valid object file");

    bfd_map_over_sections(abfd, run_sect, &out);
}

} // gob

