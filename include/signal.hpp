#pragma once

#ifndef C_SIGNAL_HPP
#define C_SIGNAL_HPP

#include <memory>

class Signal 
{
public:
	Signal(unsigned int size);
	Signal(const Signal& other);
	~Signal() = default;

	double& operator[](unsigned int index);
	double operator[](unsigned int index) const;

	std::unique_ptr<double> data;
	unsigned int size;
};

#endif