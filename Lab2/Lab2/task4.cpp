#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <windows.h>
using namespace std;

queue<int> numbers;
mutex m;
condition_variable cv;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

void DataPreparation() {
    vector<int> input;
    int x;
    cout << "Вводьте цілі числа (будь-яка літера - кінець вводу):\n";
    while (cin >> x) {
        input.push_back(x);
    }
    {
        lock_guard<mutex> lk(m);
        for (int v : input) numbers.push(v);
    }
    cv.notify_one();
}

void DataProcessing() {
    unique_lock<mutex> lk(m);
    cv.wait(lk, [] { return !numbers.empty(); });

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