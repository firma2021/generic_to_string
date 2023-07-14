#include "to_str.hpp"
using namespace std;

int one() { return 1; }

using type1 = int (&())();
using type2 = int (&)();

int main()
{
    // ifstream fin{"/home/firma/cpp_workspace/universal_print/test.cpp"};
    // cout << fin.rdbuf();
    // int i;
    // fin >> i;
    // console_print(fin);
    // i = 123;
    // console_print(i);
    // console_print(123);
    // console_print(vector<int>{1, 2, 3});
    cout << "11111" << endl;

    cout << type_str(one);
}
