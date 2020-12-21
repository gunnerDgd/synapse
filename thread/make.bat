cl.exe /c /EHsc /I ../../ ./pool/thread_pool.cpp /MDd

lib thread_pool.obj
del thread_pool.obj

move thread_pool.lib ../lib