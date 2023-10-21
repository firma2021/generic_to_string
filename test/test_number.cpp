#include "../to_str.hpp"
#include <ios>

int main()
{
    const volatile int i {1};
    const double       d {3.14};
    float              f {2023.10};

    gout << i << '\n';
    gout << d << '\n';
    gout << f << '\n';

    const bool    tr {true};
    volatile bool fa {false};

    gout << boolalpha;
    gout << tr << '\n';
    gout << fa << '\n';
}
