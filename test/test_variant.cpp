#include <generic_to_string.hpp>
#include <variant>

struct Foo
{
    int  i {1};
         Foo() = default;
         Foo(const Foo& foo) { throw std::domain_error("copy ctor"); }
    Foo& operator=(const Foo&) = default;
};

int main()
{
    variant<monostate, Foo> a;
    gout << a << '\n';

    const variant<float, double, char, int, string> b = "hello, world!"s;
    gout << b << '\n';

    try
    {
        a = Foo();
    }
    catch (...)
    {
    }

    gout << a << '\n';
}
