#include <chrono>
#include <generic_to_string.hpp>

int main()
{
    auto beg {chrono::steady_clock::now()};

    auto now {chrono::system_clock::now()};
    gout << now << '\n';

    auto end {chrono::steady_clock::now()};
    gout << (end - beg) << '\n';
}
