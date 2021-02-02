#include <synapse/lockfree/queue.hpp>
#include <thread>

int main()
{
	lockfree::cqueue<int, 1000> cq;
	synchronous::event th_evt;
	
	std::cout << "Initialized\n";
	
	auto th_func = [&](int st, int end)
	{
		for( ; st <= end ; st++)
			cq.enqueue(st);
	};
	
	std::thread t1(th_func, 1, 250),
	t2(th_func, 251, 500),
	t3(th_func, 501, 750),
	t4(th_func, 751, 1000);
	
	th_evt.alert();
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	
	size_t sum = 0;
	for(int i = 0 ; i < 1000 ; i++) {
		auto v = cq.dequeue();
		if(v.has_value())
			sum += v.value();
	}
	
	std::cout << sum << std::endl;
	
	return 0;
}