#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <chrono>
#include <windows.h>
using namespace std;

bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int nthPrime(int n) {
    int count = 0, x = 1;
    while (count < n) {
        x++;
        if (isPrime(x)) count++;
    }
    return x;
}

void Thread1(int n, promise<int> first, promise<bool> signal, promise<int> second) {
    first.set_value(nthPrime(n));
    signal.set_value(true);
    second.set_value(nthPrime(n * 10));
}

void Thread2(int n, future<bool> signal) {
    signal.get();
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Потік 2: sqrt(" << n << ") = " << sqrt(n) << endl;
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int n;
    cout << "Введіть n: ";
    cin >> n;

    promise<int> firstProm;
    promise<bool> signalProm;
    promise<int> secondProm;

    future<int> firstFut = firstProm.get_future();
    future<bool> signalFut = signalProm.get_future();
    future<int> secondFut = secondProm.get_future();

    thread t1(Thread1, n, move(firstProm), move(signalProm), move(secondProm));
    thread t2(Thread2, n, move(signalFut));

    int first = firstFut.get();
    cout << n << "-те просте число: " << first << endl;

    int second = secondFut.get();
    cout << n * 10 << "-те просте число: " << second << endl;

    t1.join();
    t2.join();
    return 0;
}