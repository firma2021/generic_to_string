#include <functional>
#include <generic_to_string.hpp>
#include <thread>

void print(int i)
{
    for (int j = 0; j <= i; ++j)
    {
        sout(i, ": ", 'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd', '\n');
    }
}

void sync_print(int i)
{
    for (int j = 0; j <= i; ++j)
    {
        sync_sout(i, ": ", 'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd', '\n');
    }
}

int main()
{
    vector<thread> vt;
    vt.reserve(10);

    for (int i = 0; i < 10; ++i)
    {
        vt.emplace_back(print, i);
    }
    for_each(begin(vt), end(vt), mem_fn(&thread::join));
    vt.clear();

    for (int i = 0; i < 10; ++i)
    {
        vt.emplace_back(sync_print, i);
    }

    for_each(begin(vt), end(vt), mem_fn(&thread::join));

    cout << sout.str() << endl;

    gout
        << "-----------------------------" << '\n';

    cout << sync_sout.str() << endl;
}
