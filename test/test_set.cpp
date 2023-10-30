#include <generic_to_string.hpp>
#include <set>
#include <unordered_set>

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
    gout << set<int> {} << '\n';

    const set a {1, 2, 3};
    gout << a << '\n';

    const unordered_multiset b {2, 0, 2, 3};
    gout << b << '\n';

    const multiset c {
        pair {1, 3.14},
        pair {1, 3.14}
    };
    gout << c << '\n';

    set<Foo> d {
        Foo {1, 2},
        Foo {3, 4}
    };
    gout << d << '\n';

    auto hash_to_pfoo = [](const pair<Foo, Foo>& f)
    { return f.first.a + f.first.b + f.second.a + f.second.b; };
    unordered_multiset<pair<Foo, Foo>, decltype(hash_to_pfoo)> e;
    e.insert(pair<Foo, Foo> {
        Foo {1, 2},
        Foo {2, 1}
    });
    e.insert(pair<Foo, Foo> {
        Foo {3, 4},
        Foo {4, 3}
    });
    e.insert(pair<Foo, Foo> {
        Foo {5, 6},
        Foo {6, 5}
    });
    gout << e << '\n';
}
