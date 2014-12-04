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

	for(int i = s2.getOffset(); i<s2.getSize()-s2.getOffset(); ++i)
	{
		double sum = 0.0;
		for(int k = h.getOffset(); k < h.getSize()-h.getOffset(); ++k)
		{
			int i2 = i-k;
			// symétrie mirroir
			// /!\ ne marche pas si |i2| >= 2*x.getSize();
			if(i2-x.getOffset() < 0)
			{
				i2 = -(i-k-x.getOffset());
			}
			else if(i2-x.getOffset() >= x.getSize())
			{
				i2 = x.getSize() - i2 + x.getOffset();
			}
			//int i2 = (i-k - x.getOffset())%x.getSize();
			//i2 = ((i-k-x.getOffset())/x.getSize() % 2) ? x.getSize()-i2 + x.getOffset() : i2 + x.getOffset();
			sum += h[k]*x[i2];
			
		}
		s2[i] = sum;
	}

	x = s2;
	return x;
}

namespace haar
{
	void analyse(Signal& x)
	{
		std::cout << 1 << std::endl;
		Signal xb = x;
		std::cout << 1 << std::endl;
		Signal xh = x;
		std::cout << 1 << std::endl;
		Signal _h0(3);
		_h0[0] = 1/std::sqrt(2);
		_h0[1] = 1/std::sqrt(2);
		_h0[2] = 0;
		std::cout << 1 << std::endl;
		Signal _h1(3);
		_h1[0] = 1/std::sqrt(2);
		_h1[1] = -1/std::sqrt(2);
		_h1[2] = 0;
		std::cout << 1 << std::endl;
		filtrage(xb, _h0);
		std::cout << 1 << std::endl;
		filtrage(xh, _h1);
		std::cout << 1 << std::endl;
	}
}
}