../../lib/libfile.a : ./file.cpp
	g++ -c ./file.cpp -I ../../../ -fconcepts -std=c++17 -o ../../lib/linux/libfile.o
	ar rscv ../../lib/linux/libfile.a ../../lib/linux/libfile.o

clean:
	rm ../../lib/linux/*.o