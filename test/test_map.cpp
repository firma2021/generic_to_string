#include <map>
#include <to_str.hpp>

class Foo
{ };

int main()
{
    const map<int, int> a {
        {1, 2},
        {3, 4}
    };

    gout << a << '\n';

    map<string, int> b {
        {"hello", 2},
        {"world", 3}
    };

    gout << b << '\n';
}
