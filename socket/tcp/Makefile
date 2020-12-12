all : tcpserver.a tcp.a

tcpserver.a : tcp_server.cpp
	g++ -c tcp_server.cpp -I ../../../ -std=c++17 -o tcpserver.o -fconcepts -DUNIX_MODE
	ar rscv libtcpserver.a tcpserver.o

tcp.a : tcp.cpp
	g++ -c tcp.cpp -I ../../../ -std=c++17 -o tcp.o -fconcepts -DUNIX_MODE
	ar rscv libtcp.a tcp.o

clean :
	rm *.o
	mv *.a ../../lib