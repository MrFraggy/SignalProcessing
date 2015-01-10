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

	double average(const Signal& s);
	double variance(const Signal& s);
	double average(const Signal2D& s);
	double variance(const Signal2D& s);

	double debit(const double globalDebit, const double var);

	double significantError(const Signal& s1, const Signal& s2);

	void minMaxAverage(const Signal& s1, unsigned int level);
}

#endif