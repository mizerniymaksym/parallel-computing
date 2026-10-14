// Пункт 1.2.3: спільний список без синхронізації
#include <iostream>
#include <thread>
#include <list>
#include <algorithm>

using namespace std;

list<int> l;

void AddToList(int start_value) {
	for (int i = 0; i < 10; ++i) {
		l.push_back(start_value + i);
		cout << "AddToList: Added " << start_value + i << "\n";
	}
}

void ListContains(int target) {
	for (int i = 0; i < 10; ++i) {
		bool found = (find(l.begin(), l.end(), target) != l.end());
		cout << "ListContains: Try " << i + 1 << " - El " << target
			<< (found ? " found" : " not found") << "\n";
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
