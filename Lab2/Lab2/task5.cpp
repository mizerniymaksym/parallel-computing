#include <iostream>
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

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int mode;
    cout << "Режим запуску (1 - deferred, 2 - async): ";
    cin >> mode;

    int n;
    cout << "Введіть номер простого числа n: ";
    cin >> n;

    launch policy = (mode == 1) ? launch::deferred : launch::async;
    future<int> f = async(policy, nthPrime, n);

    int choice;
    cout << "Поки йде обчислення, оберіть функцію від n:\n";
    cout << "1 - корінь квадратний, 2 - синус, 3 - натуральний логарифм: ";
    cin >> choice;

    if (choice == 1) cout << "sqrt(" << n << ") = " << sqrt(n) << endl;
    else if (choice == 2) cout << "sin(" << n << ") = " << sin(n) << endl;
    else if (choice == 3) cout << "ln(" << n << ") = " << log(n) << endl;

    auto start = chrono::steady_clock::now();
    int result = f.get();
    auto end = chrono::steady_clock::now();
    double waited = chrono::duration<double>(end - start).count();

    cout << n << "-те просте число: " << result << endl;
    cout << "Час очікування на результат: " << waited << " с" << endl;
    return 0;
}