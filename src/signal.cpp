#include "signal.hpp"
#include <cstring>
#include <stdexcept>

Signal::Signal(int size, int t) : data(new double[size]), size(size), offset(t)
{
	
}

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