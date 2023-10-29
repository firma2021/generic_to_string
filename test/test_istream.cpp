#include <fstream>
#include <to_str.hpp>

int main()
{
    ifstream cur {"../../test/test_istream.cpp"};
    gout(cur, '\n');
}
