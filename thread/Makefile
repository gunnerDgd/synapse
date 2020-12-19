../../liblibthreadpool.a : ./pool/thread_pool.cpp
	g++ -c ./pool/thread_pool.cpp -I ../../ -o ../lib/libthreadpool.o -fconcepts -DUNIX_MODE -std=c++17 -lpthread
	ar rscv ../lib/libthreadpool.a ../lib/libthreadpool.o

clean:
	rm ../lib/*.o