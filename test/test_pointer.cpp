#include <cstdio>
#include <generic_to_string.hpp>

int main()
{
    gout << stdin << '\n';
    gout << stdout << '\n';
    gout << stderr << '\n';

    int a {123};
    gout(&a, '\n');

    const volatile int* const volatile p0 = &a;
    const volatile int* p1                = &a;
    int* const volatile p2                = &a;
    gout(p0, '\n', p1, '\n', p2, '\n');

    const volatile void* const volatile p3 = &a;
    gout(p3, '\n');

    gout(nullptr, '\n');
    gout(NULL, '\n');
}
