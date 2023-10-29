#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
using namespace std;

#if __has_include(<cxxabi.h>)
    #include <cxxabi.h>
constexpr bool need_demangle {true};
#else
constexpr bool need_demangle {false};
#endif

inline auto demangle_abi(const char* mangled_name)
{
    if constexpr (need_demangle)
    {
        return unique_ptr<char[]> {abi::__cxa_demangle(mangled_name, nullptr, nullptr, nullptr)};
    }
    else
    {
        return {};
    }
}

inline string get_demangled_name(const char* mangled_name)
{
    auto demangled_name {demangle_abi(mangled_name)};
    return demangled_name ? string {demangled_name.get()} : "invalid type name"s;
}

inline void get_demangled_name(ostream& os, const char* mangled_name)
{
    auto demangled_name {demangle_abi(mangled_name)};
    demangled_name ? os << demangled_name.get() : os << "invalid type name"s;
}

inline string get_typename_from_typeinfo(const type_info& ti)
{
    return get_demangled_name(ti.name());
}

inline void get_typename_from_typeinfo(ostream& os, const type_info& ti)
{
    get_demangled_name(os, ti.name());
}
