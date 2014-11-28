#include "signalprocessing.hpp"
#include "signal.hpp"
#include <iostream>

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
			int i2 = (i-k - x.getOffset())%x.getSize();
			
			// symmétrie mirroir
			i2 = ((i-k-x.getOffset())/x.getSize() % 2) ? x.getSize()-i2 + x.getOffset() : i2 + x.getOffset();
			try {
				sum += h[k]*x[i2];
			} catch(std::exception& e)
			{
				std::cout << i2 << std::endl;
			}
			
		}
		s2[i] = sum;
	}

	x = s2;
	return x;
}

}