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
	Signal signalBiortho = signal;
	tools::lifting::analyse(signalLift);
	tools::lifting::synthese(signalLift);
	tools::haar::analyse(signalHaar);
	tools::haar::synthese(signalHaar);
	tools::biortho97::analyse(signalBiortho);
	tools::biortho97::synthese(signalBiortho);
	signalLift.savepng("./data/test.png");
	signalBiortho.savepng("./data/test2.png");
	signal.savepng("./data/base.png");
	//signalHaar.savepng("./data/test2.png");
	std::cout << "Error: " << tools::significantError(signal, signal) << std::endl;
	std::cout << "Error Lift: " << tools::significantError(signal, signalLift) << std::endl;
	std::cout << "Error Haar: " << tools::significantError(signal, signalHaar) << std::endl;
	std::cout << "Error Biortho: " << tools::significantError(signal, signalBiortho) << std::endl;

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