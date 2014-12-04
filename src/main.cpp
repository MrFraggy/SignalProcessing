#include <iostream>
#include "signal.hpp"
#include "signalprocessing.hpp"

int main(void)
{
	Signal signal;
	signal.load("./data/leleccum.txt");
	tools::haar::analyse(signal);
	tools::haar::synthese(signal);
	signal.save("./data/test4");

	Signal rampe(256);
	for(int i = 0; i<rampe.getSize(); ++i)
		rampe[i] = i;
	tools::haar::analyse(rampe);
	rampe.save("./data/test5");
	return 0;
}