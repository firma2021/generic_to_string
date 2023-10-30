#include <deque>
#include <forward_list>
#include <generic_to_string.hpp>
#include <list>

int main()
{
    const vector a {1, 2, 3, 4, 5, 6, 7, 8, 9};
    gout << a << '\n';

    forward_list<string> b {"hello", "world", "!"};
    gout << std::move(b) << '\n';

    const deque<list<pair<string, int>>> c {
        {{"hello"s, 1}, {"world"s, 2}},
        { {"fuck"s, 3},  {"life"s, 4}}
    };

    gout << c << '\n';
}
