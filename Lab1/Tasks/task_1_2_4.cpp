// Пункт 1.2.4: доступ до списку захищено м'ютексом (lock/unlock)
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>

using namespace std;

list<int> l;
mutex list_mtx;

void AddToList(int start_value) {
	for (int i = 0; i < 10; ++i) {
		list_mtx.lock();
		l.push_back(start_value + i);
		cout << "AddToList: Added " << start_value + i << "\n";
		list_mtx.unlock();
	}
}

void ListContains(int target) {
	for (int i = 0; i < 10; ++i) {
		list_mtx.lock();
		bool found = (find(l.begin(), l.end(), target) != l.end());
		cout << "ListContains: Try " << i + 1 << " - El " << target
			<< (found ? " found" : " not found") << "\n";
		list_mtx.unlock();
	}
}

int main() {
	int main_value = 5;
	thread t1(AddToList, main_value);
	thread t2(ListContains, main_value);
	t1.join();
	t2.join();
	return 0;
}
