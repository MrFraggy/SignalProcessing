#include "signalprocessing.hpp"
#include "signal.hpp"
#include <iostream>
#include <cmath>

namespace tools
{

Signal& interpolation(Signal& in)
{
	Signal s2(in.getSize()*2, in.getOffset()*2);

	for(int i = in.getOffset(); i<in.getSize()-in.getOffset(); ++i)
		s2[i*2] = in[i];
	
	in = s2;
	return in;
}

Signal& decimation(Signal& in)
{
	Signal s2(in.getSize(), in.getOffset());

	for(int i = s2.getOffset()/2; i<s2.getSize()/2-s2.getOffset()/2; ++i)
		s2[i] = in[i*2];
	
	in = s2;
	return in;
}

Signal& filtrage(Signal& x, const Signal& h)
{
	Signal s2(x.getSize(), x.getOffset());

    for(int i = 0; i < s2.getSize(); ++i)
	{
		double sum = 0.0;
        for(int k = -h.getSize()/2; k <= h.getSize()/2; ++k)
        {
            int i2 = i - k;//(k - h.getSize()/2); // ! \\ Nous n'avons pas gérer/testé avec les offset !!!!
			// symétrie mirroir
			// /!\ ne marche pas si |i2| >= 2*x.getSize();
            if(i2 < 0)
            {
                i2 = - i2;
			}
            else if(i2 >= x.getSize())
            {
                i2 = 2*x.getSize() - i2 - 2;
			}

            try {
                sum += h[k + h.getSize()/2]*x[i2];
            } catch(std::exception& e)
            {
                std::cout << "out of range in filtrage: " << i << " " << i - k << " " << i2 << std::endl;
            }
		}
        s2[i] = sum;
	}

	x = s2;
	return x;
}

double significantError(const Signal& s1, const Signal& s2)
{
	double error = 0.;
	if(s1.getSize() != s2.getSize())
		throw std::string("Signals doesn't have the same size");

	if(s1.getOffset() != s2.getOffset())
		throw std::string("Signals doesn't have the same start time");

	for(int i = s1.getOffset(); i<s1.getSize()+s1.getOffset(); ++i)
	{
		error += (s1[i]-s2[i])*(s1[i]-s2[i]);
	}

	return error;
}

namespace haar
{
	void analyse(Signal& x)
	{
		Signal xb = x;
		Signal xh = x;

		Signal _h0(3);
		_h0[0] = 1/std::sqrt(2);
		_h0[1] = 1/std::sqrt(2);
		_h0[2] = 0;

		Signal _h1(3);
		_h1[0] = 1/std::sqrt(2);
		_h1[1] = -1/std::sqrt(2);
		_h1[2] = 0;

		filtrage(xb, _h0);
		filtrage(xh, _h1);

		decimation(xb);
		decimation(xh);

		const int realSize = xb.getSize()/2;
		for(int i = 0; i<realSize; ++i)
			x[i] = xb[i];
		for(int i = 0; i<realSize; ++i)
			x[realSize+i] = xh[i];
	}

	void synthese(Signal& x)
	{
		const int realSize = x.getSize()/2;
		Signal xb(realSize);
		Signal xh(realSize);
		for(int i = 0; i<realSize; ++i)
			xb[i] = x[i];
		for(int i = 0; i<realSize; ++i)
			xh[i] = x[realSize+i];

		interpolation(xb);
		interpolation(xh);

		Signal _g0(3);
		_g0[0] = 0;
		_g0[1] = 1/std::sqrt(2);
		_g0[2] = 1/std::sqrt(2);

		Signal _g1(3);
		_g1[0] = 0;
		_g1[1] = -1/std::sqrt(2);
		_g1[2] = 1/std::sqrt(2);
 		
 		filtrage(xb, _g0);
		filtrage(xh, _g1);

		for(int i = 0; i<x.getSize(); ++i)
			x[i] = xb[i]+xh[i];
	}
}

namespace biortho97
{
	void analyse(Signal& x)
	{
		Signal xb = x;
		Signal xh = x;

		// Filtre biorthogonal 9/7 _h0 (longueur 9)
		Signal _h0(9);
		_h0[0]=0.037828455507;
		_h0[1]=-0.023849465019;
		_h0[2]=-0.110624404418;
		_h0[3]=0.377402855613;
		_h0[4]=0.852698679009;
		_h0[5]=0.377402855613;
		_h0[6]=-0.110624404418;
		_h0[7]=-0.023849465019;
		_h0[8]=0.037828455507;

		// Filtre biorthogonal 9/7 _h1 (longueur 9)
		Signal _h1(9);
		_h1[0]=0.064538882629;
		_h1[1]=-0.040689417610;
		_h1[2]=-0.418092273222;
		_h1[3]=0.788485616406;
		_h1[4]=-0.418092273222;
		_h1[5]=-0.040689417610;
		_h1[6]=0.064538882629;
		_h1[7]=0.000000000000;
		_h1[8]=-0.000000000000;

		filtrage(xb, _h0);
		filtrage(xh, _h1);

		decimation(xb);
		decimation(xh);

		const int realSize = xb.getSize()/2;
		for(int i = 0; i<realSize; ++i)
			x[i] = xb[i];
		for(int i = 0; i<realSize; ++i)
			x[realSize+i] = xh[i];

	}

	void synthese(Signal& x)
	{
		const int realSize = x.getSize()/2;
		Signal xb(realSize);
		Signal xh(realSize);
		for(int i = 0; i<realSize; ++i)
			xb[i] = x[i];
		for(int i = 0; i<realSize; ++i)
			xh[i] = x[realSize+i];

		interpolation(xb);
		interpolation(xh);

		// Filtre biorthogonal 9/7 _g0 (longueur 7)
		Signal _g0(7);
		_g0[0]=-0.064538882629;
		_g0[1]=-0.040689417610;
		_g0[2]=0.418092273222;
		_g0[3]=0.788485616406;
		_g0[4]=0.418092273222;
		_g0[5]=-0.040689417610;
		_g0[6]=-0.064538882629;

		// Filtre biorthogonal 9/7 _g1 (longueur 11)
		Signal _g1(11);
		_g1[0]=0.000000000000;
		_g1[1]=-0.000000000000;
		_g1[2]=0.037828455507;
		_g1[3]=0.023849465019;
		_g1[4]=-0.110624404418;
		_g1[5]=-0.377402855613;
		_g1[6]=0.852698679009;
		_g1[7]=-0.377402855613;
		_g1[8]=-0.110624404418;
		_g1[9]=0.023849465019;
		_g1[10]=0.037828455507;

		filtrage(xb, _g0);
		filtrage(xh, _g1);

		for(int i = 0; i<x.getSize(); ++i)
			x[i] = xb[i]+xh[i];
	}
}

namespace lifting
{
	void analyse(Signal& s)
	{
		if(s.getSize()%2)
			throw std::string("Le signal doit être pair");

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
			throw std::string("Le signal doit être pair");

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
}
}
