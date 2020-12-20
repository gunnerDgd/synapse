../../lib/libfile.a : ./file.cpp
	g++ -c ./file.cpp -I ../../../ -fconcepts -std=c++17 -o ../../lib/libfile.o
	ar rscv ../../lib/libfile.a ../../lib/libfile.o

clean:
	rm ../../lib/*.o