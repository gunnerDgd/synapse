all : libtcpserver.a libtcp.a

libtcp.a : tcp.cpp
	g++ -c tcp.cpp -I ../../../ -std=c++17 -o tcp.o -fconcepts -DUNIX_MODE
	ar rc libtcp.a tcp.o

libtcpserver.a : tcp_server.cpp libtcp.a
	g++ -c tcp_server.cpp libtcp.a -I ../../../ -std=c++17 -o tcpserver.o -fconcepts -DUNIX_MODE
	ar rc libtcpserver.a tcpserver.o

clean :
	rm *.o