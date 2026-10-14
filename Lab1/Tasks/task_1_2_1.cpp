// Пункт 1.2.1: потоки без join() та detach()
#include <iostream>
#include <thread>

using namespace std;

void Thread1() {
	cout << 1;
}

void Thread2() {
	cout << 2;
}

int main() {
	thread t1(Thread1);
	thread t2(Thread2);
	// ні join(), ні detach(): деструктор joinable-потоку викликає std::terminate()
	return 0;
}
