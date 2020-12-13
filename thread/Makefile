libthreadpool.a : ./pool/thread_pool.cpp
	g++ -c ./pool/thread_pool.cpp -I ../../ -o libthreadpool.o -fconcepts -DUNIX_MODE -std=c++17 -lpthread
	ar rscv libthreadpool.a libthreadpool.o

clean:
	rm ./*.o