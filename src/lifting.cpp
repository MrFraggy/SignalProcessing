#include "lifting.hpp"
#include "signal.hpp"
#include "signal2d.hpp"
#include <iostream>
#include <sstream>
#include <cmath>

namespace lifting
{
	void analyse(Signal& s)
	{
		if(s.getSize()%2)
		{
			std::ostringstream oss;
			oss << "Le signal doit être pair: ";
			oss << s.getSize();
			throw oss.str();
		}

		Signal x = s;
		const double a[5] = { -1.586134342, -0.05298011854, 0.8829110762, 0.4435068522, 1/1.149604398};
		const int half = s.getSize()/2;

		x[2*(half-1)+1] = a[0]*s[2*(half-1)]+s[2*(half-1)+1]+a[0]*s[2*(half-1)];
		for(int n = 0; n<half-1; ++n)
		{
			// "Prédiction" 1 : Pour tout n, 
			// x[2n+1] <- a*x[2n]+x[2n+1]+a*x[2n+2]
			// avec a=-1.586134342
			x[2*n+1] = a[0]*s[2*n]+s[2*n+1]+a[0]*s[2*n+2];
		} 

		x[0] = a[1]*x[1]+x[0]+a[1]*x[1];
		for(int n = 1; n<half;++n)
		{
			// "Mise à jour" 1 : Pour tout n, 
			// x[2n] <- a*x[2n-1]+x[2n]+a*x[2n+1]
			// avec a=-0.05298011854
			x[2*n] = a[1]*x[2*n-1]+x[2*n]+a[1]*x[2*n+1];
		}

		x[2*(half-1)+1] = a[2]*x[2*(half-1)]+x[2*(half-1)+1]+a[2]*x[2*(half-1)+0];
		for(int n = 0; n<half-1; ++n)
		{
			// "Prédiction" 2 : Pour tout n, 
			// x[2n+1] <- a*x[2n]+x[2n+1]+a*x[2n+2]
			// avec a=0.8829110762
			x[2*n+1] = a[2]*x[2*n]+x[2*n+1]+a[2]*x[2*n+2];
		}

		x[0] = a[3]*x[1]+x[0]+a[3]*x[1];
		for(int n = 1; n<half; ++n)
		{	
			// "Mise à jour" 2 : Pour tout n, 
			// x[2n] <- a*x[2n-1]+x[2n]+a*x[2n+1]
			// avec a=0.4435068522
			x[2*n] = a[3]*x[2*n-1]+x[2*n]+a[3]*x[2*n+1];
		}

		for(int n = 0; n<half; ++n)
		{
			// "Mise à l'échelle" : Pour tout n, 
			// x[2n] <- x[2n]/a et 
			// x[2n+1] <- a*x[2n+1]
			// avec a=1/1.149604398
			x[2*n] = x[2*n]/a[4];
			x[2*n+1] = a[4]*x[2*n+1];
		}

		for(int n = 0; n<half; ++n)
		{
			// "Mise en forme" : Mettez les valeurs d'index pairs dans la première moitié de x 
			// et les valeurs d'index impairs dans la seconde moitié. 
			// (exemple : 01234567 devient 02461357)
			s[n] = x[2*n];
			s[n+half] = x[2*n+1];
		}
	}

	void synthese(Signal& s)
	{
		if(s.getSize()%2)
		{
			std::ostringstream oss;
			oss << "Le signal doit être pair: ";
			oss << s.getSize();
			throw oss.str();
		}

		Signal x = s;
		const double a[5] = {1.586134342, 0.05298011854, -0.8829110762, -0.4435068522, 1.149604398};
		const int half = s.getSize()/2;

		for(int n = 0; n<half; ++n)
		{
			// "Mise en forme" : Réciproque de la "mise en forme" de la décomposition. 
			// (exemple : 02461357 devient 01234567)
			x[2*n] = s[n];
			x[2*n+1] = s[n+half];
		}

		for(int n = 0; n<half; ++n)
		{
			// "Mise à l'échelle" : Pour tout n, 
			// x[2n] <- x[2n]/a et 
			// x[2n+1] <- a*x[2n+1]
			// avec a=1.149604398
			x[2*n] = x[2*n]/a[4];
			x[2*n+1] = a[4]*x[2*n+1];
		}

		x[0] = a[3]*x[1]+x[0]+a[3]*x[1];
		for(int n = 1; n<half; ++n)
		{	
			// "Mise à jour" 2 : Pour tout n, 
			// x[2n] <- a*x[2n-1]+x[2n]+a*x[2n+1]
			// avec a=0.4435068522
			x[2*n] = a[3]*x[2*n-1]+x[2*n]+a[3]*x[2*n+1];
		}

		x[2*(half-1)+1] = a[2]*x[2*(half-1)]+x[2*(half-1)+1]+a[2]*x[2*(half-1)];
		for(int n = 0; n<half-1; ++n)
		{
			// "Prédiction" 2 : Pour tout n, 
			// x[2n+1] <- a*x[2n]+x[2n+1]+a*x[2n+2]
			// avec a=0.8829110762
			x[2*n+1] = a[2]*x[2*n]+x[2*n+1]+a[2]*x[2*n+2];
		}

		x[0] = a[1]*x[1]+x[0]+a[1]*x[1];
		for(int n = 1; n<half;++n)
		{
			// "Mise à jour" 1 : Pour tout n, 
			// x[2n] <- a*x[2n-1]+x[2n]+a*x[2n+1]
			// avec a=0.05298011854
			x[2*n] = a[1]*x[2*n-1]+x[2*n]+a[1]*x[2*n+1];
		}
		x[2*(half-1)+1] = a[0]*x[2*(half-1)]+x[2*(half-1)+1]+a[0]*x[2*(half-1)];
		for(int n = 0; n<half-1; ++n)
		{
			// "Prédiction" 1 : Pour tout n, 
			// x[2n+1] <- a*x[2n]+x[2n+1]+a*x[2n+2]
			// avec a=1.586134342
			x[2*n+1] = a[0]*x[2*n]+x[2*n+1]+a[0]*x[2*n+2];
		}
		s = x;
	}

	void analyse(Signal2D& s) {
		for(int i = 0; i<s.getSize(); ++i) {
			Signal l = s.getLine(i);
			analyse(l);
			s.setLine(l, i);
		}

		for(int i = 0; i<s.getSize(); ++i) {
			Signal l = s.getColumn(i);
			analyse(l);
			s.setColumn(l, i);
		}
	}
}