#include <to_str.hpp>

class Foo
{ };

int main()
{
    any a;
    gout << a << '\n';

    a = vector<string> {"hello,", "world!"};
    gout << a << '\n';

    a = Foo {};
    gout << a << '\n';

    const any b;
    gout << b << '\n';
}
