#include <bits/stdc++.h>
using namespace std;

class MyClass
{
public:
    // 其他成员函数和数据成员

    // 重载 operator<<，接收 iomanip 类型的参数
    MyClass& operator<<(std::ostream& (*manip_arg)(std::ostream&))
    {
        manip_arg(os_);
        return *this;
    }

    // 重载 operator<<，接收其他任意类型的参数
    template <typename T>
    MyClass& operator<<(const T& value)
    {
        os_ << value;
        return *this;
    }

    MyClass(ostream& os):
    os_(os) { }

private:
    std::ostream& os_;
};

int main()
{
    MyClass myClass(cout);

    myClass << std::setw(10) << std::setfill('*') << "Hello";
    myClass << 42;

    return 0;
}
