#pragma once

#ifndef C_SIGNAL_HPP
#define C_SIGNAL_HPP

#include <memory>

class Signal 
{
public:
	Signal(int size, int t = 0);
	Signal(const Signal& other);
	~Signal() = default;

	double& operator[](int index);
	double operator[](int index) const;

protected:
	std::unique_ptr<double> data;
	int size;
	int offset;
};

#endif