#include <functional>
#include <generic_to_string.hpp>

struct Foo
{
    int a;
    int b;
};

struct Zoo
{
    int a;
    int b;

    friend ostream& operator<<(ostream& os, const Zoo& zoo)
    {
        return os << "zoo: " << zoo.a << ", " << zoo.b << '\n';
    }
};

int main()
{
    const Foo f {1, 2};
    const Zoo z {3, 4};

    gout << f << '\n';
    gout << z << '\n';

    gout << function<int(int)>() << '\n';
}
