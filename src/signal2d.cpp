#include "signal2d.hpp"
#include "bmp.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>

Signal2D::Signal2D(int size) : 
	data(new double[size*size]), size(size) 
{
	std::memset(data.get(), 0, sizeof(double)*size);
}

Signal2D::Signal2D(const Signal2D& other) 
	: Signal2D(other.size)
{
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size*size);
}

Signal2D& Signal2D::operator=(const Signal2D& other)
{
	size = other.size;
	data.reset(new double[size*size]);
	std::memcpy(data.get(), other.data.get(), sizeof(double)*size*size);
	return *this;
}

double Signal2D::operator[](unsigned int i) const
{
	return data.get()[i];
}

double& Signal2D::operator[](unsigned int i)
{
	return data.get()[i];
}

bool Signal2D::load(const std::string& filename) 
{
	unsigned int tmp;
	data.reset(charge_bmp256(filename.c_str(), (unsigned int*)&size, &tmp));
	return data != nullptr;
}

bool Signal2D::save(const std::string& filename) 
{
	return ecrit_bmp256(filename.c_str(), size, size, data.get()) == 1;
}

void Signal2D::fill(const Signal2D& s, int x, int y, int length)
{
	for(int i = 0; i<length; ++i)
	{
		for(int j = 0; j<length; ++j)
		{
			int px = i+x, py = j+y;
			data.get()[py*size+px] = s[py*length+px];
		}
	}
}


void Signal2D::setLine(const Signal& s, int line)
{
	for(int i = 0; i<size; ++i)
		data.get()[i+line*size] = s[i];
}

void Signal2D::setColumn(const Signal& s, int col)
{
	for(int i = 0; i<size; ++i)
		data.get()[col+i*size] = s[i];
}

Signal Signal2D::getLine(int line) 
{
	Signal s(size);
	for(int i = 0; i<size; ++i)
		s[i] = data.get()[i+line*size];
	return s;
}

Signal Signal2D::getColumn(int col) 
{
	Signal s(size);
	for(int i = 0; i<size; ++i)
		s[i] = data.get()[col+i*size];
	return s;
}

int Signal2D::getSize() const 
{
	return size;
}

Signal2D Signal2D::subSignal(int x, int y, int length)
{
	Signal2D s(length);
	for(int i = 0; i<length; ++i)
		for(int j = 0; j<length; ++j)
		{
			int px = i+x, py = j+y;
			s[py*length+px] = data.get()[py*size+px];
		}
	
	return s;
}