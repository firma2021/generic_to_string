#include <generic_to_string.hpp>
#include <memory>

int main()
{
    const unique_ptr<const int> a {new int {2022}};
    shared_ptr<double>          b {new double {3.14}};
    weak_ptr<double>            c {b};

    gout << a << '\n';

    gout << std::move(b) << '\n';

    gout << c << '\n';
    gout << c.lock() << '\n';

    {
        auto p = std::move(b);
    }

    gout << c << '\n';
    gout << c.lock() << '\n';
}