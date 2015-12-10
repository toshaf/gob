#pragma once

#include <map>
#include <string>
#include <exception>

std::map<std::string, int> load_symbols(char const* path);

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

