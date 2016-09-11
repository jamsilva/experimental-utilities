CXXFLAGS=-std=c++11 -O3 -c
CXXFLAGSDSG=${CXXFLAGS} -DDISABLE_SHARED_GLOBAL
LDFLAGS=-Wl,-O1

ab: compile link-ab
ab-sg: compile-sg link-ab
ba: compile link-ba
ba-sg: compile-sg link-ba

compile-sg:
	clang++ testa.cpp -o testa.o ${CXXFLAGS}
	clang++ testb.cpp -o testb.o ${CXXFLAGS}

compile:
	clang++ testa.cpp -o testa.o ${CXXFLAGSDSG}
	clang++ testb.cpp -o testb.o ${CXXFLAGSDSG}

link-ab:
	clang++ testa.o testb.o -o test ${LDFLAGS}

link-ba:
	clang++ testb.o testa.o -o test ${LDFLAGS}
