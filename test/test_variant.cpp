#include <generic_to_string.hpp>
#include <variant>

int main()
{
    variant<int> a {1};
    gout << a << '\n';
    cout << variant_size<remove_cvref_t<decltype(a)>>::value;
}
