#pragma once

#include <map>
#include <string>
#include <exception>

struct symbol {
    symbol(std::string name, int addr)
        : name(name)
        , addr(addr)
    {}
    std::string name;
    int addr;
};

std::map<std::string, symbol> load_symbols(char const* path);

struct gob_exception
    : public std::exception {
    gob_exception(char const* m)
        : msg(m) {}
    virtual const char* what() const noexcept {
        return msg.c_str();
    }
private:
    std::string msg;
};

template<typename F>
struct defer {
    defer(F f) : func(f) {}
    ~defer() { func(); }
private:
    F func;
};

