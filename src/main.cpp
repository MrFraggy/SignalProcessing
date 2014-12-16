#include <iostream>
#include "signal.hpp"
#include "signalprocessing.hpp"
#include "bmp.hpp"

int main(void)
{
	////////////////////////////////////////////////
	///	DEBUG
	////////////////////////////////////////////////

	// Signal signal(8);
	// for(int i = 0; i<signal.getSize(); ++i)
	// 	signal[i] = i;

	// Signal filtre(5);
	// filtre[2] = 1;
	// Signal cpy = signal;
	// tools::filtrage(cpy, filtre);
	// cpy.save("test.dat");
	

	//////////////////////////////////////////////
	/// LELECUM
	//////////////////////////////////////////////

	Signal signal;
	signal.load("./data/leleccum.txt");

	tools::haar::analyse(signal);
	signal.savepng("./data/leleccum/haaranalyse.png");
	tools::haar::synthese(signal);
	signal.savepng("./data/leleccum/haar.png");
	tools::biortho97::analyse(signal);
	signal.savepng("./data/leleccum/biortho97analyse.png");
	tools::biortho97::synthese(signal);
	signal.savepng("./data/leleccum/biortho97.png");

	/*
	Signal signalHaar = signal;
	Signal signalLift = signal;
	Signal signalBiortho = signal;
	Signal signalAMR = signal;
	tools::lifting::analyse(signalLift);
	tools::lifting::synthese(signalLift);
	tools::haar::analyse(signalHaar);
	tools::haar::synthese(signalHaar);
	tools::biortho97::analyse(signalBiortho);
	tools::biortho97::synthese(signalBiortho);
	tools::amr::analyse(signalBiortho, 1);
	tools::amr::synthese(signalBiortho, 1);
	signalLift.savepng("./data/test.png");
	signalLift.save("./data/test1.dat");
	signalBiortho.savepng("./data/test2.png");
	signalBiortho.save("./data/test2.dat");
	signalHaar.savepng("./data/test3.png");
	signalHaar.save("./data/test3.dat");
	signal.save("./data/test.dat");
	signal.savepng("./data/base.png");
	//signalHaar.savepng("./data/test2.png");
	std::cout << "Error: " << tools::significantError(signal, signal) << std::endl;
	std::cout << "Error Lift: " << tools::significantError(signal, signalLift) << std::endl;
	std::cout << "Error Haar: " << tools::significantError(signal, signalHaar) << std::endl;
	std::cout << "Error Biortho: " << tools::significantError(signal, signalBiortho) << std::endl;
	*/

	//////////////////////////////////////////////
	/// RAMPE
	//////////////////////////////////////////////

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

	//////////////////////////////////////////////
	/// LENA SIGNAL
	//////////////////////////////////////////////

	Signal lena512;
	lena512.load("./data/lena.txt");

	tools::haar::analyse(lena512);
	lena512.savepng("./data/lena512/haaranalyse.png");
	tools::haar::synthese(lena512);
	lena512.savepng("./data/lena512/haar.png");
	tools::biortho97::analyse(lena512);
	lena512.savepng("./data/lena512/biortho97analyse.png");
	tools::biortho97::synthese(lena512);
	lena512.savepng("./data/lena512/biortho97.png");
	tools::lifting::analyse(lena512);
	lena512.savepng("./data/lena512/liftinganalyse.png");
	tools::lifting::synthese(lena512);
	lena512.savepng("./data/lena512/lifting.png");

	/*
		Q : Tracez et commentez les courbes des coefficients.
		Décrivez les interêts du "lifting".

		R : même resultat mais plus facile à réutiliser/implementer?
	*/

	// AMR
	try{
		tools::amr::analyse(lena512, 1);
		lena512.savepng("./data/lena512/amranalyse.png");
		tools::amr::synthese(lena512, 1);
		lena512.savepng("./data/lena512/amr.png");
	} catch(const std::string& s)
	{
		std::cerr << "EXCEPTION: " << s << std::endl;
	}
	//////////////////////////////////////////////
	/// LENA BMP
	//////////////////////////////////////////////

	return 0;
}