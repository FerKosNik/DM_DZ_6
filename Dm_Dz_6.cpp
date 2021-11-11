
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

once_flag callflag;

void once_print()
{
    cout << '!';
}
void print(size_t x)
{
    call_once(callflag, once_print);
    cout << x;
}

int main()
{
    vector<thread> v;
    for (size_t i = 0; i < 10; ++i) {
        v.push_back(thread(print, i));
    }
    for (auto& t : v) { t.join(); }
    cout << endl;
}
