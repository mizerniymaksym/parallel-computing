// Пункт 1.2.2: потоки з detach()
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
	t1.detach();
	t2.detach();
	// main не чекає потоки: вивід може бути "12", "21", "1", "2" або порожнім
	return 0;
}
