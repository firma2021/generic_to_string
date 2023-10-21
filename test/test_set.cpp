#include <set>
#include <to_str.hpp>
#include <unordered_set>

struct Foo
{
    int a {};
    int b {};
        Foo() = default;
        Foo(int a, int b):
    a {a}, b {b} { }
    bool operator==(const Foo& other) const
    {
        return other.a == a && other.b == b;
    }
};

int main()
{
    const set a {1, 2, 3};
    gout << a << '\n';

    const unordered_multiset b {2, 0, 2, 3};
    gout << b << '\n';

    const multiset c {
        pair {1, 3.14},
        pair {1, 3.14}
    };
    gout << c << '\n';

    auto hash_to_foo = [](const Foo& f)
    { return hash<int> {}(f.a) + hash<int> {}(f.b); };
    unordered_set<Foo, decltype(hash_to_foo)> d {
        Foo {1, 2},
        Foo {3, 4}
    };
    gout << d << '\n';

    auto hash_to_pfoo = [](const pair<Foo, Foo>& f)
    { return hash<int> {}(f.first.a) + hash<int> {}(f.second.b); };
    unordered_multiset<pair<Foo, Foo>, decltype(hash_to_pfoo)> e;
    gout << e << '\n';
}
