#include "signal.hpp"
#include <cstring>
#include <stdexcept>

Signal::Signal(unsigned int size) : data(new double[size]), size(size)
{
	
}

Signal::Signal(const Signal& other) : Signal(size)
{
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size);
}

double& Signal::operator[](unsigned int index)
{
	if(index >= size)
		throw std::out_of_range("out of range");
	return data.get()[index];
}

double Signal::operator[](unsigned int index) const
{
	if(index >= size)
		throw std::out_of_range("out of range");
	return data.get()[index];
}