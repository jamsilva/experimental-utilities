#include "test.hpp"

#ifndef DISABLE_SHARED_GLOBAL
	SHARED_GLOBAL_DEF(hello);
#else
	std::string hello("Hello");
#endif

int main()
{
	std::cout << hello << std::endl;
}
