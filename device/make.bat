cl.exe /c /EHsc /I ../../ ./file/file.cpp /MDd
lib file.obj
del file.obj
copy libfile.lib ../lib