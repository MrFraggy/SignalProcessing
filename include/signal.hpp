#pragma once

#ifndef C_SIGNAL_HPP
#define C_SIGNAL_HPP

#include <memory>

class Signal 
{
public:
	Signal(int size = 0, int t = 0);
	Signal(const Signal& other);
	~Signal() = default;

	double& operator[](int index);
	double operator[](int index) const;

	bool load(const std::string& filename);
	bool save(const std::string& filename);

protected:
	std::unique_ptr<double> data;
	int size;
	int offset;
};

#endif