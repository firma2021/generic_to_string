#include <fstream>
#include <generic_to_string.hpp>
#include <iterator>
#include <string>

int main()
{
    ifstream fin {"../../../test/test_istream.cpp"};
    gout << fin << '\n';

    int a {};
    fin >> a;
    gout << fin << '\n';
    fin.clear();

    string buf {istreambuf_iterator<char> {fin}, istreambuf_iterator<char> {}};
    gout << fin << '\n';

    fin.peek();
    gout << fin << '\n';

    fin.clear();
    gout << fin << '\n';
}
