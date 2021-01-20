../../lib/libudp.a : udp.cpp
	g++ -c udp.cpp -I ../../../ -std=c++17 -o ../../lib/linux/libudp.o -fconcepts
	ar rc ../../lib/linux/libudp.a ../../lib/linux/libudp.o

clean :
	rm ../../lib/linux/*.o