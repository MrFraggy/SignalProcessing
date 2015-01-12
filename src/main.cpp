#include <iostream>
#include "signal.hpp"
#include "signal2d.hpp"
#include "tools.hpp"
#include "amr.hpp"
#include "haar.hpp"
#include "biortho97.hpp"
#include "lifting.hpp"
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
	
	Signal signalHaar = signal;
	Signal signalLift = signal;
	Signal signalBiortho = signal;
	Signal signalAMR = signal;

	lifting::analyse(signalLift);
	signalLift.savepng("./data/leleccum/liftinganalyse.png");
	lifting::synthese(signalLift);
	signalLift.savepng("./data/leleccum/lifting.png");
	
	haar::analyse(signalHaar);
	signalHaar.savepng("./data/leleccum/haaranalyse.png");
	haar::synthese(signalHaar);
	signalHaar.savepng("./data/leleccum/haar.png");

	biortho97::analyse(signalBiortho);
	signalBiortho.savepng("./data/leleccum/biortho97analyse.png");
	biortho97::synthese(signalBiortho);
	signalBiortho.savepng("./data/leleccum/biortho97.png");

	amr::analyse(signalAMR, 1);
	signalAMR.savepng("./data/leleccum/amranalyse.png");
	amr::synthese(signalAMR, 1);
	signalAMR.savepng("./data/leleccum/amr.png");

	std::cout << "Error: " << tools::significantError(signal, signal) << std::endl;
	std::cout << "Error Lift: " << tools::significantError(signal, signalLift) << std::endl;
	std::cout << "Error Haar: " << tools::significantError(signal, signalHaar) << std::endl;
	std::cout << "Error Biortho: " << tools::significantError(signal, signalBiortho) << std::endl;

	//////////////////////////////////////////////
	/// RAMPE
	//////////////////////////////////////////////
/*
	Signal rampe(256);
	for(int i = 0; i<rampe.getSize(); ++i)
		rampe[i] = i;

	haar::analyse(rampe);
	rampe.savepng("./data/rampe/haaranalyse.png");
	haar::synthese(rampe);
	rampe.savepng("./data/rampe/haar.png");
	biortho97::analyse(rampe);
	rampe.savepng("./data/rampe/biortho97analyse.png");
	biortho97::synthese(rampe);
	rampe.savepng("./data/rampe/biortho97.png");*/

	//////////////////////////////////////////////
	/// LENA SIGNAL 1D
	//////////////////////////////////////////////
	
	Signal lena1D;
	lena1D.load("./data/lena.txt");
	
	haar::analyse(lena1D);
	lena1D.savepng("./data/lena1D/haaranalyse.png");
	haar::synthese(lena1D);
	lena1D.savepng("./data/lena1D/haar.png");
	biortho97::analyse(lena1D);
	lena1D.savepng("./data/lena1D/biortho97analyse.png");
	biortho97::synthese(lena1D);
	lena1D.savepng("./data/lena1D/biortho97.png");
	lifting::analyse(lena1D);
	lena1D.savepng("./data/lena1D/liftinganalyse.png");
	lifting::synthese(lena1D);
	lena1D.savepng("./data/lena1D/lifting.png");

	/*
		Q : Tracez et commentez les courbes des coefficients.
		Décrivez les interêts du "lifting".
		R : Même resultat mais plus facile à réutiliser/implementer?
	*/

	//////////////////////////////////////////////
	/// LENA SIGNAL 2D
	//////////////////////////////////////////////

	Signal2D lena2D;
	lena2D.load("./data/lena.bmp");

	Signal lena512 = lena2D.getLine(255);

	//////////////////////////////////////////////
	/// AMR
	//////////////////////////////////////////////

	try{

		/*Signal lena512_1 = lena512;
		amr::analyse(lena512_1, 1);
		tools::minMaxAverage(lena512_1, 1);
		amr::synthese(lena512_1, 1);

		Signal lena512_2 = lena512;
		amr::analyse(lena512_2, 2);
		tools::minMaxAverage(lena512_2, 2);
		amr::synthese(lena512_2, 2);

		Signal lena512_4 = lena512;
		amr::analyse(lena512_4, 4);
		tools::minMaxAverage(lena512_4, 4);
		lena512_4.savepng("./data/lena512/liftinganalyse_4.png");
		amr::synthese(lena512_4, 4);

		std::cout << tools::significantError(lena512_4, lena512origine) << std::endl;
	*/
		/*biortho97::analyse(lena2D);

		lena2D.save("./data/lenaBmp/test.bmp");
		Signal2D tmp = lena2D.subSignal(0,0,256);*/
		amr::analyse(lena2D, 3);
		lena2D.save("./data/lenaBmp/test.bmp");
		amr::synthese(lena2D, 3);
		lena2D.save("./data/lenaBmp/test2.bmp");

		Signal lena512_last = lena2D.getLine(255);

		std::cout << "Erreur significative : " << tools::significantError(lena512, lena512_last) << std::endl;
	} catch(const std::string& s)
	{
		std::cerr << "EXCEPTION: " << s << std::endl;
	}

	/* 
		Q: Niveau maximal de décompositions pour un signal de taille p?
		R: log2(p)
	
		Q: Montrer de manière objective que le signal reconstruit est identique
		au signal d'origine.
		R: Calcul de l'erreur significative pour les deux signaux.

		Q: Si l'on réalise une amr de niveau j combien de sous bande obtient-on ?
		R: j + 1 avec j sous bande de détails et une sous bande d'approximation.

		Q: Calculez les valeurs moyennes et les variances de chaque sous-bande obtenues par l'AMR 2D de la question précédente.
		Commentez ces valeurs. 
		R:
		Analyse 64: 
			Moyenne dl : 126.69
			Variance dl : 32.3168
			Moyenne dc : 127.483
			Variance dc : 14.8526
			Moyenne dd : 127.146
			Variance dd : 4.1833

		Analyse 128: 
			Moyenne dl : 126.95
			Variance dl : 0.488768
			Moyenne dc : 126.888
			Variance dc : 19.6341
			Moyenne dd : 127.048
			Variance dd : 0.125768

		Analyse 256: 
			Moyenne dl : 126.786
			Variance dl : 3.36558
			Moyenne dc : 127.111
			Variance dc : 17.0578
			Moyenne dd : 127.012
			Variance dd : 1.61692


			A 		 D2			D1
		|--------|--------|----------------|	
	*/

	


	//////////////////////////////////////////////
	/// LENA BMP
	//////////////////////////////////////////////

	return 0;
}