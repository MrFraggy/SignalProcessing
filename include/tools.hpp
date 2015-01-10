#pragma once

#ifndef C_SIGNALPROCESSING_HPP
#define C_SIGNALPROCESSINH_HPP

class Signal;
class Signal2D;

namespace tools 
{
	Signal& interpolation(Signal& in);
	Signal& decimation(Signal& in);

	Signal& filtrage(Signal& x, const Signal& h);

	float maximumLevel(const Signal& s);

	void addValue(Signal2D& s, double d);
	void linearize(Signal2D& s);

	double significantError(const Signal& s1, const Signal& s2);
	void minMaxAverage(const Signal& s1, unsigned int level);
}

#endif