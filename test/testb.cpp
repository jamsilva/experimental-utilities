#include "test.hpp"

TestHello testHello;

TestHello::TestHello()
{
	std::cout << hello << std::endl;
	hello = "XYZZY";
}
