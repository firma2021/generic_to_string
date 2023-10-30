#include <generic_to_string.hpp>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Foo
{
    string      s {"hello, world!"s};
    double      d {3.14};
    vector<int> vi {1, 2, 3};
};

template <size_t N>
constexpr auto&
get(Foo& foo) noexcept
{
    if constexpr (N == 0)
    {
        return foo.s;
    }
    else if constexpr (N == 1)
    {
        return foo.d;
    }
    else if constexpr (N == 2)
    {
        return foo.vi;
    }
}

template <size_t N>
constexpr auto&&
get(Foo&& foo) noexcept
{
    if constexpr (N == 0)
    {
        return std::move(foo.s);
    }
    else if constexpr (N == 1)
    {
        return std::move(foo.d);
    }
    else if constexpr (N == 2)
    {
        return std::move(foo.vi);
    }
}

template <size_t N>
constexpr const auto&
get(const Foo& foo) noexcept
{
    if constexpr (N == 0)
    {
        return foo.s;
    }
    else if constexpr (N == 1)
    {
        return foo.d;
    }
    else if constexpr (N == 2)
    {
        return foo.vi;
    }
}

template <size_t N>
constexpr const auto&&
get(const Foo&& foo) noexcept
{
    if constexpr (N == 0)
    {
        return std::move(foo.s);
    }
    else if constexpr (N == 1)
    {
        return std::move(foo.d);
    }
    else if constexpr (N == 2)
    {
        return std::move(foo.vi);
    }
}

namespace std
{
template <>
struct tuple_element<0, Foo>
{
    using type = string;
};
template <>
struct tuple_element<1, Foo>
{
    using type = double;
};
template <>
struct tuple_element<2, Foo>
{
    using type = vector<int>;
};

template <>
struct tuple_size<Foo> : public integral_constant<size_t, 3>
{ };
}   // namespace std

void test(Tuple auto&& t)
{
}

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

    Foo f;
    const auto& [x, y, z] = f;
    gout << x << '\n';
    gout << y << '\n';
    gout << z << '\n';
}
