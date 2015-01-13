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

	std::cout << "Error de test (signal avec lui même): " << tools::significantError(signal, signal) << std::endl;
	std::cout << "Error Lift: " << tools::significantError(signal, signalLift) << std::endl;
	std::cout << "Error Haar: " << tools::significantError(signal, signalHaar) << std::endl;
	std::cout << "Error Biortho: " << tools::significantError(signal, signalBiortho) << std::endl;
	std::cout << "Error AMR: " << tools::significantError(signal, signalAMR) << std::endl;

	//////////////////////////////////////////////
	/// LENA SIGNAL 1D
	//////////////////////////////////////////////
	
	Signal lena1D;
	lena1D.load("./data/lena.txt");

	signalHaar = lena1D;
	signalBiortho = lena1D;
	signalLift = lena1D;
	signalAMR = lena1D;
	
	haar::analyse(signalHaar);
	signalHaar.savepng("./data/lena1D/haaranalyse.png");
	haar::synthese(signalHaar);
	signalHaar.savepng("./data/lena1D/haar.png");
	biortho97::analyse(signalBiortho);
	signalBiortho.savepng("./data/lena1D/biortho97analyse.png");
	biortho97::synthese(signalBiortho);
	signalBiortho.savepng("./data/lena1D/biortho97.png");
	lifting::analyse(signalLift);
	signalLift.savepng("./data/lena1D/liftinganalyse.png");
	lifting::synthese(signalLift);
	signalLift.savepng("./data/lena1D/lifting.png");

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
		{
			Signal2D lenaSave = lena2D;
			Signal2D lena2D_quant = lena2D;
			Signal2D lena2D_noquant = lena2D;
			amr::analyse(lena2D_quant, 3);
			amr::analyse(lena2D_noquant, 3);
			auto debits = tools::computeDebit(lena2D_quant, 3, 4);
			tools::quantifiate(lena2D_quant, 3, debits);

			{
				Signal2D lenaCopy = lena2D_quant;
				tools::arrange(lenaCopy, 3);
				lenaCopy.save("./data/lenaBmp/amr_analyse_quant4.bmp");
			}
			{
				Signal2D lenaCopy = lena2D_noquant;
				tools::arrange(lenaCopy, 3);
				lenaCopy.save("./data/lenaBmp/amr_analyse_noquant.bmp");
			}
			amr::synthese(lena2D_quant, 3);
			amr::synthese(lena2D_noquant, 3);
			lena2D_quant.save("./data/lenaBmp/amr_analyse_quant4.bmp");
			lena2D_noquant.save("./data/lenaBmp/amr_synthese_noquant.bmp");
		
			std::cout << "PSNR 4bpp: " << tools::psnr(lena2D_quant, lenaSave) << std::endl;
			tools::encode(lena2D_quant, 3, debits, "./data/lenaBmp/compressed4.bin");	
		}
		/*
		{
			Signal2D lenaSave = lena2D;
			Signal2D lena2D_quant = lena2D;
			amr::analyse(lena2D_quant, 3);
			auto debits = tools::computeDebit(lena2D_quant, 3, 2);
			tools::quantifiate(lena2D_quant, 3, debits);

			{
				Signal2D lenaCopy = lena2D_quant;
				tools::arrange(lenaCopy, 3);
				lenaCopy.save("./data/lenaBmp/amr_analyse_quant2.bmp");
			}

			amr::synthese(lena2D_quant, 3);
			lena2D_quant.save("./data/lenaBmp/amr_synthese_quant2.bmp");

			std::cout << "PSNR 2bpp: " << tools::psnr(lena2D_quant, lenaSave) << std::endl;
		}

		{
			Signal2D lenaSave = lena2D;
			Signal2D lena2D_quant = lena2D;
			amr::analyse(lena2D_quant, 3);
			auto debits = tools::computeDebit(lena2D_quant, 3, 1);
			tools::quantifiate(lena2D_quant, 3, debits);

			{
				Signal2D lenaCopy = lena2D_quant;
				tools::arrange(lenaCopy, 3);
				lenaCopy.save("./data/lenaBmp/amr_analyse_quant1.bmp");
			}
			amr::synthese(lena2D_quant, 3);
			lena2D_quant.save("./data/lenaBmp/amr_synthese_quant1.bmp");

			std::cout << "PSNR 1bpp: " << tools::psnr(lena2D_quant, lenaSave) << std::endl;
		}
*/
		{
			Signal2D lenaSave = lena2D;
			Signal2D lena2D_quant = lena2D;
			amr::analyse(lena2D_quant, 3);

			auto debits = tools::computeDebit(lena2D_quant, 3, 0.5);
			tools::quantifiate(lena2D_quant, 3, debits);
			{
				Signal2D lenaCopy = lena2D_quant;
				tools::arrange(lenaCopy, 3);
				lenaCopy.save("./data/lenaBmp/amr_analyse_quant05.bmp");
			}
			amr::synthese(lena2D_quant, 3);
			lena2D_quant.save("./data/lenaBmp/amr_synthese_quant05.bmp");
			std::cout << "PSNR 0.5bpp: " << tools::psnr(lena2D_quant, lenaSave) << std::endl;

			tools::encode(lena2D_quant, 3, debits, "./data/lenaBmp/compressed0_5.bin");
		}
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
			Moyenne da : 988.064
			Variance d : 128009
			Moyenne dl : -0.947905
			Variance dl : 3485.32
			Moyenne dc : -0.000835034
			Variance dc : 1092.59
			Moyenne dd : -0.329082
			Variance dd : 1065.01
			Moyenne des variances: 33413.1
	
		Analyse 128: 
			Moyenne da : 246.696
			Variance d : 216627
			Moyenne dl : -0.0131988
			Variance dl : 170.159
			Moyenne dc : 0.121534
			Variance dc : 443.677
			Moyenne dd : 0.071866
			Variance dd : 115.94
			Moyenne des variances: 54339.1

		Analyse 256: 
			Moyenne da : 61.7192
			Variance d : 65742.3
			Moyenne dl : 0.0462509
			Variance dl : 42.9312
			Moyenne dc : 0.00810543
			Variance dc : 17.217
			Moyenne dd : 0.00859221
			Variance dd : 8.67861
			Moyenne des variances: 16452.8

			A 		 D2			D1
		|--------|--------|----------------|	

		Q: Si l'on réalise une AMR2D de niveau j, combien de sous-bandes obtient-on?
		R: j*3 sous-bandes de détail + 1 sous-bande d'approximation

		
	*/
	return 0;
}