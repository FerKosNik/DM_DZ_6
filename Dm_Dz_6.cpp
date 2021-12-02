#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <list>
#include <mutex>
#include <string>

#include <chrono>

class Timer
{
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::string m_name;
    std::chrono::time_point<clock_t> m_beg;
    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }

public:
    Timer() : m_beg(clock_t::now()) { }
    Timer(std::string name) : m_name(name), m_beg(clock_t::now()) { }

    void start(std::string name) {
        m_name = name;
        m_beg = clock_t::now();
    }
    void print() const {
        std::cout << m_name << ":\t" << elapsed() * 1000 << " ms" << '\n';
    }
};

using namespace std;

void fun_print(string str);

static mutex mtx_pcout;

class pcout {
public:

    pcout() {
        mtx_pcout.lock();
    }
    ~pcout() {
        mtx_pcout.unlock();
    }

    template<typename T>
    pcout& operator<<(const T& _t);
    pcout& operator<<(ostream& (*text)(ostream&));
};
void task1() {

    pcout() << "TASK1\n";

    thread t1(fun_print, "This thread name is t1");
    thread t2(fun_print, "This thread name is t2");
    thread t3(fun_print, "This thread name is t3");
    thread t4(fun_print, "This thread name is t4");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    pcout() << "\n-----------------------------------------\n";
}


template<typename T>
pcout& pcout::operator<<(const T& _t) {
    cout << _t;
    return *this;
}

void fun_print(string str) {
    this_thread::sleep_for(2s);
    pcout() << str << endl;
}


pcout& pcout::operator<<(ostream& (*text)(ostream&))
{
    cout << text;
    return *this;
}


//-------------------------------//

void printProgress(int& tmp_Quant, const int& req_Quant);
void countSimples(const int& Num, int& nSimple );

void task2(const int& Num) {
    cout << "\nTASK2\n";

    int tmp_num = 1;
    int nSimple = 1;

    thread th1(countSimples, ref(Num), ref(nSimple));
    thread th2(printProgress, ref(nSimple), ref(Num));

    th1.join();
    th2.join();
}


void countSimples(const int& Num, int& nSimple) {
    unsigned long long n;
    n = Num;
    unsigned long long pout;
    
    vector <unsigned long long> a(n + 1);
    for (unsigned long long i = 0; i < n + 1; i++)
        a[i] = i;
    for (unsigned long long p = 2; p < n + 1; p++)
    {
        if (a[p] != 0)
        {
            pout = p;
            
            for (unsigned long long j = p * p; j < n + 1; j += p)
                if (j < n + 1) { a[j] = 0; };
        }
        nSimple = p;
    }
    cout << " Max prime number = " << a[pout] << endl;

}


void printProgress(int& tmp_Quant, const int& req_Quant) {
    int currentPercent = 0;
    int old_Percent = 0;

    while (tmp_Quant < req_Quant)
    {
        currentPercent = tmp_Quant * 100 / req_Quant;
        if (currentPercent > old_Percent) {
            cout << "Progress: " << currentPercent << "%\n";
            old_Percent = currentPercent;
        }
    }
}

//-------------------------------//
void owner(set <int>& house, bool& res);

void thief(set <int>& house, bool& res);

void task3() {
    cout << "\nTASK3\n";
    
    set <int> house = { 5600, 230, 5400, 12, 340, 1200, 990, 87, 53 };

    bool end = false;

    cout << "\nInitial quantity is " << house.size() << endl;

    Timer timer("time_of_game");


    thread t1(owner, ref(house), ref(end));
    thread t2(thief, ref(house), ref(end));

    t1.join();
    t2.join();

    cout << endl;
    timer.print();

    cout << "\nThief win\n";
    cout << "\n-----------------------\n";
}

void owner(set <int>& house, bool& res) {
    while (!res) {
        
        if (house.empty()) res = true;
        else {
            house.insert (rand()%10000);
            for (auto h : house) cout << h << " ";
            cout << endl;
        }
        this_thread::sleep_for(1500ms);
    }
}

void thief(set <int>& house, bool& res) {
    while (!res) {
        auto it = house.end();
        --it;
        house.erase(it);
        for (auto h : house) cout << h << " ";
        cout << endl;
        this_thread::sleep_for(1s);
    }
}

//-------------------------------//

int main() {

    task1();

    task2(15'000'000);
    
    task3();

    return 0;
}