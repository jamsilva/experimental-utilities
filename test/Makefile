ab: compile
	clang++ testa.o testb.o -o test

ba: compile
	clang++ testb.o testa.o -o test

compile:
	clang++ testa.cpp -o testa.o -c -std=c++11
	clang++ testb.cpp -o testb.o -c -std=c++11
