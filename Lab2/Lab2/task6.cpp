#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <deque>
#include <vector>
#include <windows.h>
using namespace std;

deque<packaged_task<int(int)>> tasks;
deque<int> numbers;
mutex m;
condition_variable cv;
bool finished = false;

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

void Worker() {
    while (true) {
        packaged_task<int(int)> task;
        int n;
        {
            unique_lock<mutex> lk(m);
            cv.wait(lk, [] { return !tasks.empty() || finished; });
            if (tasks.empty() && finished) break;
            task = move(tasks.front());
            tasks.pop_front();
            n = numbers.front();
            numbers.pop_front();
        }
        task(n);
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    thread worker(Worker);

    vector<int> ns;
    vector<future<int>> results;
    int n;

    while (true) {
        cout << "Введіть n (будь-яка літера - стоп): ";
        if (!(cin >> n)) break;

        packaged_task<int(int)> task(nthPrime);
        results.push_back(task.get_future());
        ns.push_back(n);
        {
            lock_guard<mutex> lk(m);
            tasks.push_back(move(task));
            numbers.push_back(n);
        }
        cv.notify_one();
    }

    {
        lock_guard<mutex> lk(m);
        finished = true;
    }
    cv.notify_one();

    for (size_t i = 0; i < results.size(); i++) {
        cout << ns[i] << "-те просте число: " << results[i].get() << endl;
    }

    worker.join();
    cout << "Робочий потік завершено" << endl;
    return 0;
}