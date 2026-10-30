#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <windows.h>
using namespace std;

mutex m;
condition_variable cv;
int i = 0;

void ThreadFunc(int id) {
    unique_lock<mutex> lk(m);
    cout << "Потік " << id << ": очікую сигнал\n";
    cv.wait(lk, [] { return i == 1; });
    cout << "Повідомлення з потоку " << id << "\n";
}

void Notify() {
    this_thread::sleep_for(chrono::milliseconds(100));
    {
        lock_guard<mutex> lk(m);
        i = 1;
        cout << "Notify: пробуджую один потік\n";
    }
    cv.notify_one();
}

int main() {
    SetConsoleOutputCP(1251);

    thread t1(ThreadFunc, 1);
    thread t2(ThreadFunc, 2);
    thread t3(ThreadFunc, 3);
    thread n(Notify);

    t1.detach();
    t2.detach();
    t3.detach();

    n.join();
    this_thread::sleep_for(chrono::milliseconds(100));
    return 0;
}