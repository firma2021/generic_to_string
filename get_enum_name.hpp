#pragma once

#include "get_type_name.hpp"
#include <iostream>
#include <map>
#include <source_location>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
using namespace std;

template <typename T>
concept Enum = is_enum_v<T>;

template <Enum T, T t>
constexpr auto get_enum_name_at()
{
    const string_view func_name {source_location::current().function_name()};
    auto              begin {func_name.find_last_of('=') + 2};
    auto              end {func_name.find_last_of(']')};
    return func_name.substr(begin, end - begin);
}

template <size_t N = 0>
void get_all_enum_name(Enum auto arg, vector<string_view>& names)
{
    if constexpr (N >= 64)
    {
        return;
    }
    else
    {
        auto name {get_enum_name_at<decltype(arg), static_cast<decltype(arg)>(N)>()};
        if (!isdigit(name.back()))
        {
            names.push_back(name);
            get_all_enum_name<N + 1>(arg, names);
        }
    }
}

void get_enum_name(ostream& os, Enum auto arg)
{
    static map<string_view, vector<string_view>> cache;   // enum type to enum name

    auto enum_type {get_type_name<underlying_type_t<decltype(arg)>>()};
    auto iter {cache.find(enum_type)};
    if (iter != end(cache))
    {
    }
    else
    {
        vector<string_view> names;
        get_all_enum_name(arg, names);
        iter = cache.emplace(enum_type, std::move(names)).first;
    }

    auto& names {iter->second};
    if (static_cast<size_t>(arg) >= names.size())
    {
        os << "out of range"s;
        return;
    }

    auto enum_name {names.front().substr(0, names.front().find_first_of(':'))};
    os << enum_name << '<' << enum_type << ',' << names.size() << '>';
    os << "::";
    auto cur_name {names.at(static_cast<size_t>(arg))};
    os << cur_name.substr(cur_name.find_last_of(':') + 1);
}

string get_enum_name(Enum auto arg)
{
    ostringstream os;
    get_enum_name(os, arg);
    return os.str();
}
