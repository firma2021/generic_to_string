#include <generic_to_string.hpp>
#include <utility>

int main()
{
    const tuple<> a;
    tuple         b {1, 3.14, 2023.0F};
    const tuple   c {make_pair(2.48, "hello"s), make_pair(6.96, "world"s)};

    gout << a << '\n';
    gout << b << '\n';
    gout << c << '\n';
}
