#include "amr.hpp"
#include "lifting.hpp"
#include "biortho97.hpp"
#include "tools.hpp"
#include "signal.hpp"
#include "signal2d.hpp"
#include <iostream>
#include <cmath>

namespace amr
{
	void analyse(Signal& s, int niveau)
	{
		if(niveau <= 0 || s.getSize() <= 1)	return;

		lifting::analyse(s);

		Signal tmp(s.getSize()/2);

		tmp.fill(s, 0, s.getSize()/2);

		analyse(tmp, niveau-1);

		s.fill(tmp, 0, tmp.getSize());
	}

	void syntheseRecurs(Signal& s, int niveau, int size)
	{
		size /= 2;
//		size = N/(2^(niveaumax - niveau))
		if(niveau <= 0 || size <= 0) return;

		syntheseRecurs(s, niveau-1, size);
		size *= 2;
		
		Signal tmp(size);

		tmp.fill(s, 0, size);

		lifting::synthese(tmp);

		s.fill(tmp, 0, tmp.getSize());
	}

	void synthese(Signal& s, int niveau)
	{
		syntheseRecurs(s, niveau, s.getSize());
	}



	void analyse(Signal2D& s, int niveau) {
		if(niveau <= 0 || s.getSize() <= 1)
			return;
		
		auto tmpSize = s.getSize()/2;
		biortho97::analyse(s);

		Signal2D tmp(s.subSignal(0,0,tmpSize));
		tools::linearize(tmp);

		analyse(tmp, niveau-1);

		/*for(int x = 0; x<size; ++x)
		{
			for(int y = 0; y<size; ++y)
			{
				if(x < size/2 && y < size/2)
					continue;
				tmp[x*size+y] += 127;
			}
		}*/

		Signal2D dl(s.subSignal(tmpSize, 0, tmpSize));
		Signal2D dc(s.subSignal(0, tmpSize, tmpSize));
		Signal2D dd(s.subSignal(tmpSize, tmpSize, tmpSize));
		
		tools::addValue(dl, 127); tools::addValue(dc,127); tools::addValue(dd,127);

		s.fill(dl, tmpSize, 0, tmpSize);
		s.fill(dc, 0, tmpSize, tmpSize);
		s.fill(dd, tmpSize, tmpSize, tmpSize);

		s.fill(tmp, 0, 0, tmp.getSize());
	}

	void synthese2DRecurs(Signal2D& s, int niveau, int size)
	{
		size /= 2;
//		size = N/(2^(niveaumax - niveau))
		if(niveau <= 0 || size <= 0) return;

		synthese2DRecurs(s, niveau-1, size);

		/*for(int x = 0; x<size; ++x)
		{
			for(int y = 0; y<size; ++y)
			{
				if(x < size/2 && y < size/2)
					continue;
				tmp[x*size+y] -= 127;
			}
		}*/

		Signal2D dl(s.subSignal(size, 0, size));
		Signal2D dc(s.subSignal(0, size, size));
		Signal2D dd(s.subSignal(size, size, size));
		
		tools::addValue(dl, -127); tools::addValue(dc, -127); tools::addValue(dd, -127);

		s.fill(dl, size, 0, size);
		s.fill(dc, 0, size, size);
		s.fill(dd, size, size, size);

		size *= 2;
		Signal2D tmp = s.subSignal(0,0,size);

		biortho97::synthese(tmp);

		tools::linearize(tmp);
		
		s.fill(tmp, 0, 0, size);
	}

	void synthese(Signal2D& s, int niveau)
	{
		synthese2DRecurs(s, niveau, s.getSize());
	}

	float maximumLevel(const Signal& s)
	{
		return std::log2(static_cast<float>(s.getSize()));
	}
}