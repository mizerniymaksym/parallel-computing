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

void Waits(int id) {
    unique_lock<mutex> lk(m);
    cout << "Потік " << id << ": входжу в стан очікування\n";
    cv.wait(lk, [] { return i == 1; });
    cout << "Потік " << id << ": очікування завершене\n";
}

void Awake() {
    this_thread::sleep_for(chrono::milliseconds(100));
    {
        lock_guard<mutex> lk(m);
        cout << "Awake: повідомляю решту потоків\n";
    }
    cv.notify_all();

    this_thread::sleep_for(chrono::milliseconds(100));
    {
        lock_guard<mutex> lk(m);
        i = 1;
        cout << "Awake: повторне повідомлення\n";
    }
    cv.notify_all();
}

int main() {
    SetConsoleOutputCP(1251);

    thread w1(Waits, 1);
    thread w2(Waits, 2);
    thread w3(Waits, 3);
    thread a(Awake);

    a.join();
    w1.join();
    w2.join();
    w3.join();
    return 0;
}