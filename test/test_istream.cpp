#include <fstream>
#include <to_str.hpp>

int main()
{
    ifstream cur {"/home/firma/桌面/universal_print/test/test_istream.cpp"};
    gout(cur, '\n');
}
