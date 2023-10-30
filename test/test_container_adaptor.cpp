#include <generic_to_string.hpp>
#include <queue>
#include <stack>

int main()
{
    priority_queue<int> pq;
    pq.push(2022);
    pq.push(12);
    pq.push(18);

    stack<int> stk;
    stk.push(2022);
    stk.push(12);
    stk.push(18);

    queue<int> que;
    que.push(2022);
    que.push(12);
    que.push(18);

    const queue<int> empty;

    gout(pq, '\n', stk, '\n', que, '\n', empty, '\n');
}
