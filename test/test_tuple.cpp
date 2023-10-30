#include <generic_to_string.hpp>
#include <utility>

int main()
{
    const tuple<> a;
    const tuple   b {1, 3.14, 2023.0F};
    tuple         c {make_pair(2.48, "hello"s), make_pair(6.96, "world"s)};
    const array   d {1, 2, 3, 4, 5, 6, 7, 8, 9};

    gout << a << '\n';
    gout << b << '\n';
    gout << std::move(c) << '\n';
    gout << d << '\n';
}
