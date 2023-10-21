#include <chrono>
#include <to_str.hpp>

int main()
{
    auto now {chrono::steady_clock::now()};

    gout << now << '\n';
}
