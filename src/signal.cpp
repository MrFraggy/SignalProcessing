#include "signal.hpp"
#include <cstring>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <limits>

Signal::Signal(int size, int t) : data(new double[size]), size(size), offset(t) {}

Signal::Signal(const Signal& other) : Signal(size)
{
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size);
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

	out.flags(std::ios::scientific);
	// out.precision(std::numeric_limits<double>::digits10 + 1);
	out.precision(7);

	for(int i = 0; i < size; ++i)
	{
		out << data.get()[i];
		out << std::endl;
	}

	return true;
}