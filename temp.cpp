#include <iostream>
#include <tuple>
#include <type_traits>

enum class MyEnum
{
    Value1,
    Value2,
    Value3
};

template <typename EnumType>
constexpr auto get_enum_count()
{
    return std::tuple_size<std::tuple<EnumType>> {};
}

int main()
{
    constexpr auto count = get_enum_count<MyEnum>();
    std::cout << count << std::endl;   // 输出 3，表示枚举类中有3个枚举值

    return 0;
}
