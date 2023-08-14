#include "Queue.h"
#include <string>

int main(int argc, char** argv)
{
	JC::Queue<int> test(3);
	test.push(1);
	test.push(2);
	test.push(3);
	test.push(4);
	test.print();
	std::cout << "\n";

	std::cout << "Removing: " << test.pop() << "\n";
	test.push(5);
	test.print();

	std::cout << "Removing: " << test.pop() << "\n";
	test.print();

	std::cout << "Removing: " << test.pop() << "\n";
	test.print();

	test.push(6);
	test.push(7);
	test.push(8);
	test.print();

	std::cout << "\nRemoving: " << test.pop() << "\n";
	test.print();

	std::cout << "------------------ String Queue ------------------\n";
	JC::Queue<std::string> strQ(2);

	strQ.push("One");
	strQ.push("Two");
	strQ.push("Three");
	strQ.print();

	std::cout << "\nRemoving: " << strQ.pop() << "\n";
	strQ.print();
	std::cout << "\nRemoving: " << strQ.pop() << "\n";
	strQ.print();
	std::cout << "\nRemoving: " << strQ.pop() << "\n";
	strQ.print();
	
	std::cout << "\nAdding 4, 5, 6\n";
	strQ.push("Four");
	strQ.push("Five");
	strQ.push("Six");

	strQ.print();

	return 0;
}