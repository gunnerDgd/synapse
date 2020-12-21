cl.exe /c /EHsc /I ../../ ./tcp/tcp.cpp /MDd
cl.exe /c /EHsc /I ../../ ./tcp/server/tcp_server.cpp /MDd

lib tcp.obj
lib tcp_server.obj

del tcp.obj
del tcp_server.obj

move tcp.lib ../lib
move tcp_server.lib ../lib