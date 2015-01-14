#pragma once

#ifndef C_SIGNALPROCESSING_HPP
#define C_SIGNALPROCESSINH_HPP

#include <vector>
#include <string>

class Signal;
class Signal2D;

namespace tools 
{
	Signal& interpolation(Signal& in);
	Signal& decimation(Signal& in);

	Signal& filtrage(Signal& x, const Signal& h);

	void addValue(Signal2D& s, double d);
	void linearize(Signal2D& s);

	double average(const Signal& s);
	double variance(const Signal& s);
	double average(const Signal2D& s);
	double variance(const Signal2D& s);

	double debit(const double globalDebit, const double var);

	double significantError(const Signal& s1, const Signal& s2);
	double significantError(const Signal2D& s1, const Signal2D& s2);

	double psnr(const Signal2D& s1, const Signal2D& s2);
	
	void minMaxAverage(const Signal& s1, unsigned int level);

	void arrange(Signal2D& s, unsigned int level);

	std::vector<double> computeDebit(const Signal2D& s, int level, double debitGlobal);

	void quantifiate(Signal2D& s, int level, std::vector<double> debits);

	void encode(const Signal2D& s, int level, std::vector<double> debits, const std::string& filename);
}

#endif