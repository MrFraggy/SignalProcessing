#include <iostream>
#include "signal.hpp"
#include "signalprocessing.hpp"

int main(void)
{
	Signal signal;
	Signal dirac(5);
	dirac[0] = 1;
	signal.load("./data/leleccum.txt");
	tools::interpolation(signal);
	signal.save("./data/test2");
	tools::decimation(signal);
	signal.save("./data/test3");
	tools::filtrage(signal, dirac);
	signal.save("./data/test4");
	signal.savepng("./data/test.png");

	return 0;
}