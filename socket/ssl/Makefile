../../lib/linux/libtls.a : tls.cpp
	g++ -c tls.cpp -I ../../../ -std=c++17 -fconcepts -o ../../lib/linux/libtls.o -lssl -lcrypto
	ar rc ../../lib/linux/libtls.a ../../lib/linux/libtls.o
	
clean:
	rm ../../lib/linux/*.o