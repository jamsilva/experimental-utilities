#include <iostream>
#include <string>

#include "../sharedglobal.hpp"

struct TestHello
{
	TestHello();
};

SHARED_GLOBAL_DECL(std::string, hello, ("Hello"));
extern TestHello testHello;
