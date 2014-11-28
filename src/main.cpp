#include <iostream>
#include "signal.hpp"
#include "signalprocessing.hpp"

int main(void)
{
	Signal signal;

	signal.load("./data/leleccum.txt");

	signal.save("./data/test");

	return 0;
}