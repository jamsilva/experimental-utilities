#include <iostream>
#include <string>

#ifndef DISABLE_SHARED_GLOBAL
	#include "../sharedglobal.hpp"
#endif

struct TestHello
{
	TestHello();
};

#ifndef DISABLE_SHARED_GLOBAL
	SHARED_GLOBAL_DECL(std::string, hello, ("Hello"));
#else
	extern std::string hello;
#endif

extern TestHello testHello;
