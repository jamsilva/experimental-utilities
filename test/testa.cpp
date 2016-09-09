#include "test.hpp"

SHARED_GLOBAL_DEF(hello);

TestHello testHello;

int main()
{
	std::cout << hello << std::endl;
}
