#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <algorithm>
#include <string>

using namespace std;

list<int> l;
mutex list_mtx;

void AddToList(int value) {
	lock_guard<mutex> lock(list_mtx);
	l.push_back(value);
	cout << "AddToList: Added " << value << "\n";
}

void ListContains(int target) {
	lock_guard<mutex> lock(list_mtx);
	bool found = (find(l.begin(), l.end(), target) != l.end());
	cout << "ListContains: El " << target << (found ? " found" : " not found") << "\n";
}

class someData {
public:
	string name;
	string surname;
	string address;
	int age;

	someData(string n, string s, string addr, int a)
		: name(n), surname(s), address(addr), age(a) {}

	void print() const {
		cout << name << " " << surname << ", " << address << ", " << age;
	}
};

class exchangePerson {
public:
	someData data;
	mutex m;

	exchangePerson(string n, string s, string addr, int a)
		: data(n, s, addr, a) {}

	static void JohnDoe(exchangePerson& p) {
		lock_guard<mutex> lock(p.m);
		p.data.name = "John";
		p.data.surname = "Doe";
		p.data.address = "Unknown";
		p.data.age = 120;
	}

	static void JacobSmith(exchangePerson& p) {
		lock_guard<mutex> lock(p.m);
		p.data.name = "Jacob";
		p.data.surname = "Smith";
		p.data.address = "Known";
		p.data.age = 1;
	}

	static void Swap(exchangePerson& p1, exchangePerson& p2) {
		if (&p1 == &p2) return;

		unique_lock<mutex> lock1(p1.m, defer_lock);
		unique_lock<mutex> lock2(p2.m, defer_lock);
		lock(lock1, lock2);

		cout << "\n--- Swap ---\n";
		cout << "Before: Ob.1 = "; p1.data.print();
		cout << " | Ob.2 = "; p2.data.print(); cout << "\n";

		swap(p1.data, p2.data);

		cout << "After:  Ob.1 = "; p1.data.print();
		cout << " | Ob.2 = "; p2.data.print(); cout << "\n";
	}
};

int main() {
	int main_value = 5;
	cout << "Start threads with list" << endl;
	for (int i = 0; i < 10; ++i) {
		thread t1(AddToList, main_value + i);
		thread t2(ListContains, main_value);
		t1.detach();
		t2.detach();
	}

	this_thread::sleep_for(chrono::milliseconds(500));

	exchangePerson p1("Initial1", "Last1", "Addr1", 20);
	exchangePerson p2("Initial2", "Last2", "Addr2", 30);

	thread tj(exchangePerson::JohnDoe, ref(p1));
	thread tjs(exchangePerson::JacobSmith, ref(p2));
	tj.detach();
	tjs.detach();

	this_thread::sleep_for(chrono::milliseconds(100));

	thread ts(exchangePerson::Swap, ref(p1), ref(p2));
	ts.join();

	return 0;
}
