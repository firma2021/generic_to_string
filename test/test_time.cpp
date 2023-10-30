#include <chrono>
#include <generic_to_string.hpp>

int main()
{
    auto now {chrono::steady_clock::now()};

    gout << now << '\n';
}
