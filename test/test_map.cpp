#include <generic_to_string.hpp>
#include <map>
#include <string_view>
#include <unordered_map>

struct Foo
{
    int a {};
    int b {};
        Foo() = default;
        Foo(int a, int b):
    a {a}, b {b} { }
    bool operator<(const Foo& other) const
    {
        return other.a < a && other.b < b;
    }
    bool operator==(const Foo& other) const
    {
        return other.a == a && other.b == b;
    }
    friend ostream& operator<<(ostream& os, const Foo& foo)
    {
        return os << foo.a + foo.b;
    }
};

int main()
{
    gout << map<int, string> {} << '\n';

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

    const unordered_multimap<string, vector<string>> c {
        { "first"s,      {"f", "i", "r", "s", "t"}},
        {"second"s, {"s", "e", "c", "o", "n", "d"}},
        { "third"s,      {"t", "h", "r", "i", "d"}}
    };

    gout << c << '\n';

    auto hash_to_foo = [](const Foo& f)
    { return hash<int> {}(f.a) + hash<int> {}(f.b); };

    const map<Foo, int> d {
        {Foo(1, 2), 1},
        {Foo(3, 4), 2},
        {Foo(1, 2), 3},
        {Foo(3, 4), 4}
    };

    gout << d << '\n';

    const unordered_map<Foo, int, decltype(hash_to_foo)> e {
        {Foo(1, 2), 1},
        {Foo(3, 4), 2},
        {Foo(1, 2), 3},
        {Foo(3, 4), 4}
    };

    gout << e << '\n';
}
