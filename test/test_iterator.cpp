#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <to_str.hpp>
#include <unordered_map>
#include <unordered_set>

int main()
{
    gout << vector<int> {}.begin() << '\n';
    gout << array<int, 3> {}.begin() << '\n';
    gout << string {}.begin() << '\n';
    gout << deque<int> {}.begin() << '\n';
    gout << forward_list<int> {}.begin() << '\n';
    gout << list<int> {}.begin() << '\n';
    gout << set<int> {}.begin() << '\n';
    gout << multiset<int> {}.begin() << '\n';
    gout << unordered_set<int> {}.begin() << '\n';
    gout << unordered_multiset<int> {}.begin() << '\n';
    gout << map<int, int> {}.begin() << '\n';
    gout << multimap<int, int> {}.begin() << '\n';
    gout << unordered_map<int, int> {}.begin() << '\n';
    gout << unordered_multimap<int, int> {}.begin() << '\n';
}
