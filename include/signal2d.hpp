#pragma once

#ifndef C_SIGNAL2D_HPP
#define C_SIGNAL2D_HPP

#include <memory>

#include "signal.hpp"

class Signal2D 
{
public:
	Signal2D(int size = 0);
	Signal2D(const Signal2D& other);
	~Signal2D() = default;

	Signal2D& operator=(const Signal2D& other);

	double operator[](unsigned int i) const;
	double& operator[](unsigned int i);

	bool load(const std::string& filename);
	bool save(const std::string& filename);

	void fill(const Signal2D& s, int x, int y, int length);

	void setLine(const Signal& s, int line);
	void setColumn(const Signal& s, int col);

	Signal getLine(int l);
	Signal getColumn(int c);

	int getSize() const;

	Signal2D subSignal(int x, int y, int length);

protected:
	std::unique_ptr<double> data;
	int size;
};

#endif