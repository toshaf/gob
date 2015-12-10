#pragma once

#include <map>
#include <string>
#include <functional>

#include "gob.hh"

namespace gob {

template<typename F>
struct defer {
    defer(F f) : func(f) {}
    ~defer() { func(); }
private:
    F func;
};

} // gob

std::map<std::string, gob::Symbol> load_symbols(std::string path);

void gob_trace(std::string path, std::map<gob::Address, gob::Symbol> bps, std::function<void(gob::Symbol)> handler);

