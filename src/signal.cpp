#include "signal.hpp"
#include "gnuplot_c.h"
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <limits>

Signal::Signal(int size, int t) : data(new double[size]), size(size), offset(t)
{
	std::memset(data.get(), 0, sizeof(double)*size);
}

Signal::Signal(const Signal& other) : Signal(other.size)
{
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size);
}

Signal& Signal::operator=(const Signal& other)
{
	size = other.size;
	offset = other.offset;
	data.reset(new double[size]);
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size);
	return *this;
}

double& Signal::operator[](int index)
{
	int off = index-offset;
	if(off >= size || off < 0)
		throw std::out_of_range("out of range");
	return data.get()[off];
}

double Signal::operator[](int index) const
{
	int off = index-offset;
	if(off >= size || off < 0)
		throw std::out_of_range("out of range");
	return data.get()[off];
}

bool Signal::load(const std::string& filename)
{
	std::ifstream in(filename);

	if(!in || !in.is_open()) return false;

	std::vector<double> tmp;
	double d;

	while(in >> d)
	{
		tmp.push_back(d);
	}

	data.reset(new double[tmp.size()]);

	std::memcpy(data.get(), tmp.data(), sizeof(double) * tmp.size());
	size = tmp.size();

	return true;
}

bool Signal::save(const std::string& filename)
{
	std::ofstream out(filename);

	if(!out || !out.is_open()) return false;

	out.flags(std::ios::scientific); // for exponential
	out.precision(7); // 7 digits

	for(int i = 0; i < size; ++i)
	{
		out << data.get()[i];
		out << std::endl;
	}

	return true;
}

bool Signal::savepng(const std::string& filename)
{
	/*auto *plot = gpc_init_2d("Signal", "X", "Y", 1.f, GPC_SIGNED, GPC_MULTIPLOT, GPC_KEY_DISABLE);
	gpc_plot_2d(plot, data.get(), size, "Signal", -offset, size+offset, "lines", "red", GPC_NEW);
	gpc_close(plot);*/
	
	return false;
}

int Signal::getSize() const
{
	return size;
}

int Signal::getOffset() const
{
	return offset;
}