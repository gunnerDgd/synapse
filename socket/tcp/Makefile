all : ../../lib/libtcpserver.a ../../lib/libtcp.a

../../lib/libtcp.a : tcp.cpp
	g++ -c tcp.cpp -I ../../../ -std=c++17 -o ../../lib/linux/libtcp.o -fconcepts -DUNIX_MODE
	ar rc ../../lib/linux/libtcp.a ../../lib/linux/libtcp.o

../../lib/libtcpserver.a : ./server/tcp_server.cpp ../../lib/linux/libtcp.a
	g++ -c ./server/tcp_server.cpp -o ../../lib/linux/libtcpserver.o -I ../../../ -std=c++17 -fconcepts -DUNIX_MODE
	ar rc ../../lib/linux/libtcpserver.a ../../lib/linux/libtcpserver.o

clean :
	rm ../../lib/linux/*.o