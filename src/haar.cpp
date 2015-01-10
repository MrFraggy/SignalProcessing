#include "haar.hpp"
#include "signal.hpp"
#include "tools.hpp"
#include <iostream>
#include <cmath>

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

		Signal _g0(3);
		_g0[0] = 0;
		_g0[1] = 1/std::sqrt(2);
		_g0[2] = 1/std::sqrt(2);

		Signal _g1(3);
		_g1[0] = 0;
		_g1[1] = -1/std::sqrt(2);
		_g1[2] = 1/std::sqrt(2);
 		
 		tools::filtrage(xb, _g0);
		tools::filtrage(xh, _g1);

		for(int i = 0; i<x.getSize(); ++i)
			x[i] = xb[i]+xh[i];
	}
}