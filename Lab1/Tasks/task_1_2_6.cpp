// Пункт 1.2.6: Swap через std::lock + lock_guard з adopt_lock
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

using namespace std;

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

		lock(p1.m, p2.m);
		lock_guard<mutex> lock1(p1.m, adopt_lock);
		lock_guard<mutex> lock2(p2.m, adopt_lock);

		cout << "\n--- Swap ---\n";
		cout << "Before: Ob.1 = "; p1.data.print();
		cout << " | Ob.2 = "; p2.data.print(); cout << "\n";

		swap(p1.data, p2.data);

		cout << "After:  Ob.1 = "; p1.data.print();
		cout << " | Ob.2 = "; p2.data.print(); cout << "\n";
	}
};

int main() {
	exchangePerson p1("Initial1", "Last1", "Addr1", 20);
	exchangePerson p2("Initial2", "Last2", "Addr2", 30);

	thread tj(exchangePerson::JohnDoe, ref(p1));
	thread tjs(exchangePerson::JacobSmith, ref(p2));
	tj.detach();
	tjs.detach();

	// від'єднані потоки мають завершитись до Swap і до знищення p1, p2
	this_thread::sleep_for(chrono::milliseconds(100));

	thread ts(exchangePerson::Swap, ref(p1), ref(p2));
	ts.join();

	return 0;
}
