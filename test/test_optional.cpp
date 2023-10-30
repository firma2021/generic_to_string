#include <generic_to_string.hpp>

int main()
{
    const optional<string> a {};
    gout << a << '\n';

    optional<map<int, string>> b {
        map<int, string> {{1, "hello"s}, {2, "world"s}}
    };
    gout << b << '\n';
}
