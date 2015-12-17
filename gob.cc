#include <bfd.h>
#include <string>
#include <vector>
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

SourceInfo Gob::get_source_info(Symbol sym) const {
    bfd_init();

    bfd *abfd = bfd_openr(path.c_str(), NULL);
    if (!abfd)
        throw gob_exception("Couldn't load file");
    auto cleanup = [=](){ bfd_close(abfd); };
    defer<decltype(cleanup)> d(cleanup);

    if (!bfd_check_format(abfd, bfd_object))
        throw gob_exception("Not a valid object file");

    int symtab_size = bfd_get_symtab_upper_bound(abfd);

    std::vector<asymbol*> asymtab(symtab_size / sizeof(asymbol*));
    int numsymbols = bfd_canonicalize_symtab(abfd, asymtab.data());

    for (auto sect = abfd->sections; sect != NULL; sect = sect->next) {
        if (bfd_get_section_flags(abfd, sect) & SEC_HAS_CONTENTS == 0)
            continue;

        auto vma = bfd_get_section_vma(abfd, sect);
        if (sym.addr < vma)
            continue;

        auto size = bfd_get_section_size(sect);
        if (sym.addr >= vma + size)
            continue;

        const char *filename;
        const char *funcname;
        unsigned int lineno;
        auto found = bfd_find_nearest_line(
            abfd,
            sect,
            asymtab.data(), 
            sym.addr - vma, // the section doesn't include the vma
            &filename,
            &funcname,
            &lineno);

        if (found)
            return SourceInfo{ funcname, filename, lineno };
    }

    throw gob_exception("Could not determine source location");
}

} // gob

