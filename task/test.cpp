#include <iostream>
#include <synapse/task/task.hpp>

void test_1(context::task_enumerator&);
void test_2(context::task_enumerator&);

int main()
{
	context::task_enumerator tenum;
	context::task t1(tenum, test_1);
}

void test_1(context::task_enumerator& tenum)
{
	std::cout << "Test 1 #1\n";
	context::task t2 (tenum, test_2);
	std::cout << "Test 1 #2\n";
	tenum.switch_task(test_1, test_2);
}

void test_2(context::task_enumerator& tenum)
{
	std::cout << "Test 2 #1\n";
	tenum.switch_task(test_2, test_1);
	std::cout << "Test 2 #2\n";
}