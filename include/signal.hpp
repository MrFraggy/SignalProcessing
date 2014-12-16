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

	Signal& operator=(const Signal& other);

	double& operator[](int index);
	double operator[](int index) const;

	bool load(const std::string& filename);
	bool save(const std::string& filename);

	bool savepng(const std::string& filename);

	void fill(const Signal& other, int start, int size);

	int getSize() const;
	int getOffset() const;

protected:
	std::unique_ptr<double> data;
	int size;
	int offset;
};

#endif