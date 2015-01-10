#include "biortho97.hpp"
#include "signal.hpp"
#include "signal2d.hpp"
#include "tools.hpp"
#include <iostream>
#include <cmath>

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

		tools::filtrage(xb, _h0);
		tools::filtrage(xh, _h1);

		tools::decimation(xb);
		tools::decimation(xh);

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

		tools::interpolation(xb);
		tools::interpolation(xh);

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

		tools::filtrage(xb, _g0);
		tools::filtrage(xh, _g1);

		for(int i = 0; i<x.getSize(); ++i)
			x[i] = xb[i]+xh[i];
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

	void synthese(Signal2D& s) {
		for(int i = 0; i<s.getSize(); ++i) {
			Signal l = s.getColumn(i);
			synthese(l);
			s.setColumn(l, i);
		}

		for(int i = 0; i<s.getSize(); ++i) {
			Signal l = s.getLine(i);
			synthese(l);
			s.setLine(l, i);
		}
	}
}