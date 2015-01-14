#pragma once

#ifndef C_AMR_HPP
#define C_AMR_HPP

class Signal;
class Signal2D;

namespace amr
{
	void analyse(Signal& s, int niveau);
	void synthese(Signal& s, int niveau);

	void analyse(Signal2D& s, int niveau);
	void synthese(Signal2D& s, int niveau);

	float maximumLevel(const Signal& s);
}

#endif