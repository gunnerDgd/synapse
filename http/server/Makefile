../../lib/libhttpserver.a: server.cpp
	g++ -c ./server.cpp ../../lib/libtcp.a ../../lib/libtcpserver.a -I ../../../ -o ../../lib/libhttpserver.o -fconcepts -DUNIX_MODE -std=c++17 -lpthread
	ar rscv ../../lib/libhttpserver.a ../../lib/libhttpserver.o

clean:
	rm ../../lib/*.o