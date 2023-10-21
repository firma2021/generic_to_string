#include <source_location>
#include <string_view>
using namespace std;

template <typename T>
constexpr auto get_type_name()
{
    string_view func_name {source_location::current().function_name()};
    auto        begin {func_name.find_first_of('=') + 1};
    auto        end {func_name.find_last_of(']')};
    return func_name.substr(begin, end - begin);
}

constexpr string_view get_type_name(auto&& arg)
{
    return get_type_name<decltype(arg)>();
}
