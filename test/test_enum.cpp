#include <generic_to_string.hpp>

enum class Color : unsigned char
{
    Green,
    Red,
    Blue,

};

enum class Empty : size_t
{
};

int main()
{
    const volatile Color color = Color::Red;
    gout << color << '\n';
    gout << Color::Green << '\n';
    gout << Color::Blue << '\n';
    gout << Color {} << '\n';
    gout << Color {64} << '\n';
    gout << Empty {} << '\n';
}
