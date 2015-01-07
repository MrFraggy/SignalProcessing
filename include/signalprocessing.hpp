#pragma once

#ifndef C_SIGNALPROCESSING_HPP
#define C_SIGNALPROCESSINH_HPP

class Signal;
class Signal2D;

namespace tools 
{
namespace haar 
{
	/*
		
	*/
	void analyse(Signal& s);
	void synthese(Signal& s);
}

namespace biortho97
{
	void analyse(Signal& s);
	void synthese(Signal& s);

	void analyse(Signal2D& s);
	void synthese(Signal2D& s);
}

namespace lifting
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}

namespace amr
{
	void analyse(Signal& s, int niveau);
	void synthese(Signal& s, int niveau);
	float niveauMaximum(const Signal& s);


	void analyse(Signal2D& s, int niveau);
	void synthese(Signal2D& s, int niveau);
}

Signal& interpolation(Signal& in);
Signal& decimation(Signal& in);
Signal& filtrage(Signal& x, const Signal& h);

double significantError(const Signal& s1, const Signal& s2);
void minMaxAverage(const Signal& s1, unsigned int level);
}



#endif