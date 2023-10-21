#include <to_str.hpp>
#include <variant>

int main()
{
    variant<monostate> a;
    gout << a << '\n';
    cout << variant_size<remove_cvref_t<decltype(a)>>::value;
}
