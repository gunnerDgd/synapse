cl.exe /c /EHsc /I ../../ ./server/server.cpp ../lib/tcp_server.lib /MDd
ren server.obj http_server.obj

lib http_server.obj
del http_server.obj

move http_server.lib ../lib