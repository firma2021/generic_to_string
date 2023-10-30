#include <generic_to_string.hpp>
#include <iomanip>
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

    gout(showbase, oct, 42, '\n', dec, 42, '\n', hex, 42, '\n', 42, '\n');

    gout(fixed, 0.0, '\n', scientific, 0.0, '\n', hexfloat, 0.0, '\n', defaultfloat, 0.0, '\n');
    gout(fixed, 0.01, '\n', scientific, 0.01, '\n', hexfloat, 0.01, '\n', defaultfloat, 0.01, '\n');
    gout(fixed, 0.00001, '\n', scientific, 0.00001, '\n', hexfloat, 0.00001, '\n', defaultfloat, 0.00001, '\n');
    gout(setprecision(13), fixed, 0.00001, '\n', scientific, 0.00001, '\n', hexfloat, 0.00001, '\n', defaultfloat, 0.00001, '\n');
}
