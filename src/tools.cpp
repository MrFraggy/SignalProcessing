#include "tools.hpp"
#include "signal.hpp"
#include "signal2d.hpp"
#include "quantlm.hpp"
#include "program.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#define EPSILON 0.00001
inline bool FEQUALS(double x, double y) { return std::fabs(x-y) < EPSILON; }

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
	Signal s2 = x;

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
		double tmp = (s1[i]-s2[i])*(s1[i]-s2[i]);
		error += std::isnan(tmp) ? 0 : tmp;
	}

	return error/s1.getSize();
}

double significantError(const Signal2D& s1, const Signal2D& s2)
{
	double error = 0.;
	if(s1.getSize() != s2.getSize())
		throw std::string("Signals doesn't have the same size");

	uint32_t size = s1.getSize();
	for(uint32_t i = 0; i<size*size; ++i)
	{
		double tmp = (s1[i]-s2[i])*(s1[i]-s2[i]);
		error += std::isnan(tmp) ? 0 : tmp;
	}

	return error/(size*size);
}

double psnr(const Signal2D& s1, const Signal2D& s2)
{
	double eqm = significantError(s1,s2);
	return 10*std::log10((255*255)/eqm);
}

double average(const Signal& s)
{
	double average = 0;

	for(int i = 0; i < s.getSize(); ++i)
	{
		average += s[i];
	}

	return average / s.getSize();
}

double average(const Signal2D& s)
{
	uint32_t size = s.getSize();
	double average = 0;

	for(uint32_t i = 0; i < size*size; ++i)
	{
		average += s[i];
	}

	return average / (size * size);
}

double variance(const Signal& s)
{
	double variance = 0;
	double moy = average(s);

	for(int i = 0; i < s.getSize(); ++i)
	{
		variance += (s[i] - moy) * (s[i] - moy);
	}

	return variance / (s.getSize() - 1);
}

double variance(const Signal2D& s)
{
	uint32_t size = s.getSize();
	double variance = 0;
	double moy = average(s);

	for(uint32_t i = 0; i < size*size; ++i)
	{
		variance += (s[i] - moy) * (s[i] - moy);
	}

	return variance / (size * size - 1);
}

double debit(const double globalDebit, const double var)
{
	return 0.0;
}

void minMaxAverage(const Signal& s1, unsigned int level)
{
	uint size = s1.getSize();

	int pos = 0;
	double min = s1[0], max = s1[0], average = 0;

	//coeff d'approx
	int tmpSize =(size / std::pow(2, level));
	for(int i = 0; i < tmpSize; ++i)
	{
		min = std::min(min, s1[pos]);
		max = std::max(max, s1[pos]);

		average += s1[pos];

		++pos;
	}

	average /= tmpSize;
	std::cout << "Approx (length: " << tmpSize << ") Min: " << min << " Max: " << max << " Avg: " << average << std::endl;

	// coef de details
	for(int i = level; i > 0; --i)
	{
		tmpSize = (size / std::pow(2, i));
		min = s1[pos]; max = s1[pos]; average = 0;
		for(int j = 0; j < tmpSize; ++j)
		{
			min = std::min(min, s1[pos]);
			max = std::max(max, s1[pos]);

			average += s1[pos];

			++pos;
		}
		average /= tmpSize;
		std::cout << "Detail " << level-i << " (length: " << tmpSize << ") Min: " << min << " Max: " << max << " Avg: " << average << std::endl;
	}
}

void addValue(Signal2D& s, double d)
{
	int size = s.getSize();
	for(int i = 0; i<size*size; ++i)
	{
		//if(/*FEQUALS(s[i], 255) || FEQUALS(s[i], 0))
		//	continue;
		s[i] += d;
	}
}

void linearize(Signal2D& s)
{
	uint size = s.getSize();
	// std::cout << "linearize " << size << std::endl;
	double min = s[0], max = s[0];

	// Get min & max
	for(unsigned int i = 0; i < size*size; ++i)
	{
		min = std::min(min, s[i]);
		max = std::max(max, s[i]);
	}

	// Ajust between [0; 255]
	for(unsigned int i = 0; i < size*size; ++i)
	{
		s[i] = (s[i])/(max) * 255;
	}
}

void arrange(Signal2D& s, unsigned int niveau)
{
	int size = s.getSize()/(std::pow(2,niveau));
	Signal2D tmp = s.subSignal(0,0,size);
	linearize(tmp);
	s.fill(tmp,0,0,size);
	for(uint32_t i = size; i<s.getSize(); i = i*2)
	{
		Signal2D dl(s.subSignal(i, 0, i));
		Signal2D dc(s.subSignal(0, i, i));
		Signal2D dd(s.subSignal(i, i, i));
		
		tools::addValue(dl, 127); tools::addValue(dc,127); tools::addValue(dd,127);

		s.fill(dl, i, 0, i);
		s.fill(dc, 0, i, i);
		s.fill(dd, i, i, i);
	}
}

std::vector<double> computeDebit(const Signal2D& s, int level, double debitGlobal)
{
	struct Var {
		double var;
		uint32_t size;
	};

	uint32_t size = s.getSize();
	uint32_t minSize = size/std::pow(2, level);
	std::vector<Var> variances;
	{
		Signal2D da(s.subSignal(0,0,minSize));
		variances.push_back({variance(da), minSize});
	}

	for(uint32_t i = minSize; i<size; i *= 2)
	{
		Signal2D dl(s.subSignal(i, 0, i));
		Signal2D dc(s.subSignal(0, i, i));
		Signal2D dd(s.subSignal(i, i, i));
		variances.push_back({variance(dl),i});
		variances.push_back({variance(dc),i});
		variances.push_back({variance(dd),i});
	}

	std::vector<double> debits;

	for(auto& i : variances)
	{
		double product = 1;
		for(auto& j : variances)
		{
			product *= std::pow(j.var, (j.size*j.size)*1.0/(size*size));
		}

		double bi = debitGlobal+ 0.5 * std::log2(i.var/product);
		if(bi < 0)
			bi = 0;
		debits.push_back(bi);
	}

	return debits;
/*
	for(uint32_t i = 0; i<size; ++i)
	{
		for(uint32_t j = 0; j<size; ++j)
		{

		}
	}*/
}

void quantifiate(Signal2D& s, int level, std::vector<double> debits)
{	
	uint32_t size = s.getSize();
	uint32_t minSize = size/std::pow(2, level);
	int idx = 0;
	{
		Signal2D da(s.subSignal(0,0,minSize));
		quantlm(da.get(), da.getSize()*da.getSize(), std::floor(pow(2,debits[idx++])));
		s.fill(da, 0,0,minSize);
	}

	
	for(uint32_t i = minSize; i<size; i *= 2)
	{
		Signal2D dl(s.subSignal(i, 0, i));
		Signal2D dc(s.subSignal(0, i, i));
		Signal2D dd(s.subSignal(i, i, i));
		
		quantlm(dl.get(), i*i, std::floor(pow(2,debits[idx++])));
		quantlm(dc.get(), i*i, std::floor(pow(2,debits[idx++])));
		quantlm(dd.get(), i*i, std::floor(pow(2,debits[idx++])));
		s.fill(dl, i,0,i);
		s.fill(dc, 0,i,i);
		s.fill(dd, i,i,i);
	}
}

void encode(const Signal2D& signal, int level, std::vector<double> debits, const std::string& filename)
{
	Signal2D s = signal;
	uint32_t size = s.getSize();
	uint32_t minSize = size/std::pow(2, level);
	int idx = 0;
	{
		Signal2D da(s.subSignal(0,0,minSize));
		quantlm_idx(da.get(), da.getSize()*da.getSize(), std::floor(pow(2,debits[idx++])));
		s.fill(da, 0,0,minSize);
	}

	
	for(uint32_t i = minSize; i<size; i *= 2)
	{
		Signal2D dl(s.subSignal(i, 0, i));
		Signal2D dc(s.subSignal(0, i, i));
		Signal2D dd(s.subSignal(i, i, i));
		
		quantlm_idx(dl.get(), i*i, std::floor(pow(2,debits[idx++])));
		quantlm_idx(dc.get(), i*i, std::floor(pow(2,debits[idx++])));
		quantlm_idx(dd.get(), i*i, std::floor(pow(2,debits[idx++])));
		s.fill(dl, i,0,i);
		s.fill(dc, 0,i,i);
		s.fill(dd, i,i,i);
	}

	std::ofstream file(filename, std::ios::binary);
	if(!file || !file.is_open())
	{
		std::cerr << "Cannot open file " << filename << " for writing";
		return;
	}
	for(uint32_t i = 0; i<size*size; ++i)
	{
		int16_t v = static_cast<int16_t>(s[i]);
		file.write((char*)&v, sizeof(int16_t));
	}

	Program p("gzip "+filename);
	if(!p.isValid())
	{
		std::cerr << "Cannot start gzip to compress the file" << std::endl;
		return;
	}

}	
}
