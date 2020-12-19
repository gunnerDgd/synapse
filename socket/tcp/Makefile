all : ../../lib/libtcpserver.a ../../lib/libtcp.a

../../lib/libtcp.a : tcp.cpp
	g++ -c tcp.cpp -I ../../../ -std=c++17 -o ../../lib/libtcp.o -fconcepts -DUNIX_MODE
	ar rc ../../lib/libtcp.a ../../lib/libtcp.o

../../lib/libtcpserver.a : ./server/tcp_server.cpp ../../lib/libtcp.a
	g++ -c ./server/tcp_server.cpp -o ../../lib/libtcpserver.o -I ../../../ -std=c++17 -fconcepts -DUNIX_MODE
	ar rc ../../lib/libtcpserver.a ../../lib/libtcpserver.o

clean :
	rm ../../lib/*.o