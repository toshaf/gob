#pragma once

#include <map>
#include <string>
#include <exception>
#include <functional>

namespace gob {

typedef long unsigned int Address;

struct Symbol {
    Symbol(std::string name, Address addr)
        : name(name)
        , addr(addr)
    {}
    std::string name;
    Address addr;
};

struct gob_exception
    : public std::exception {
    gob_exception(char const* m)
        : msg(m) {}
    gob_exception(std::string m)
        : msg(m) {}
    virtual const char* what() const noexcept {
        return msg.c_str();
    }
private:
    std::string msg;
};

struct Gob {
    Gob(std::string path);
    std::map<std::string, Symbol> get_symbols() const {
        return symbols;
    };
    void set_breakpoint(std::string sym_name);
    void run(std::function<void(gob::Symbol)> handler);
private:
    std::string path;
    std::map<std::string, Symbol> symbols;
    std::map<Address, Symbol> breakpoints;
};

} // gob
