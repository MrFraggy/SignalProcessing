#include <iostream>
#include "signal.hpp"
#include "signalprocessing.hpp"

int main(void)
{
	Signal signal;
	signal.load("./data/leleccum.txt");
	/*tools::haar::analyse(signal);
	signal.savepng("./data/leleccum/haaranalyse.png");
	tools::haar::synthese(signal);
	signal.savepng("./data/leleccum/haar.png");
	tools::biortho97::analyse(signal);
	signal.savepng("./data/leleccum/biortho97analyse.png");
	tools::biortho97::synthese(signal);
	signal.savepng("./data/leleccum/biortho97.png");*/
	Signal signalHaar = signal;
	Signal signalLift = signal;
	tools::lifting::analyse(signalLift);
	signalLift.savepng("./data/test2.png");
	tools::lifting::synthese(signalLift);
	tools::haar::analyse(signalHaar);
	tools::haar::synthese(signalHaar);
	signalLift.savepng("./data/test.png");
	//signalHaar.savepng("./data/test2.png");
	std::cout << "Error: " << tools::significantError(signal, signalLift) << std::endl;

	Signal rampe(256);
	for(int i = 0; i<rampe.getSize(); ++i)
		rampe[i] = i;

	tools::haar::analyse(rampe);
	rampe.savepng("./data/rampe/haaranalyse.png");
	tools::haar::synthese(rampe);
	rampe.savepng("./data/rampe/haar.png");
	tools::biortho97::analyse(rampe);
	rampe.savepng("./data/rampe/biortho97analyse.png");
	tools::biortho97::synthese(rampe);
	rampe.savepng("./data/rampe/biortho97.png");
	return 0;
}