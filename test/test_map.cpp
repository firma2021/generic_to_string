#include <generic_to_string.hpp>
#include <map>
#include <unordered_map>

class Foo
{ };

int main()
{
    const multimap<int, int> a {
        {1, 2},
        {3, 4},
        {1, 2},
        {3, 4}
    };

    gout << a << '\n';

    map<string, int> b {
        {"hello", 2},
        {"world", 3}
    };

    gout << b << '\n';

    const unordered_multimap<string, vector<string>> c {{""}, {}, {}};
}
