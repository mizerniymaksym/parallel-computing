#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <windows.h>   
using namespace std;

queue<int> numbers;   
bool ready = false;   
mutex m;              

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

void DataPreparation() {
    int x;
    cout << "Вводьте цілі числа (будь-яка літера - кінець вводу):\n";
    while (cin >> x) {              
        lock_guard<mutex> lk(m);
        numbers.push(x);
    }
    lock_guard<mutex> lk(m);
    ready = true;                   
}

void DataProcessing() {
    unique_lock<mutex> lk(m);
    while (!ready) {                
        lk.unlock();                
        this_thread::sleep_for(chrono::milliseconds(100));  
        lk.lock();                  
    }
    cout << "Прості числа: ";
    while (!numbers.empty()) {
        int x = numbers.front();
        numbers.pop();
        if (isPrime(x)) cout << x << " ";
    }
    cout << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    thread t1(DataPreparation);
    thread t2(DataProcessing);
    t1.detach();   
    t2.join();     
    return 0;
}