#pragma once

#ifndef C_HAAR_HPP
#define C_HAAR_HPP

class Signal;
class Signal2D;

namespace haar 
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}

#endif