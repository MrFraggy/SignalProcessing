#pragma once

#ifndef C_SIGNAL_HPP
#define C_SIGNAL_HPP

#include <memory>

class Signal 
{
public:
	Signal(unsigned int size);

	std::unique_ptr<double> data;
	unsigned int size;
};

#endif