#pragma once

#ifndef C_SIGNALPROCESSING_HPP
#define C_SIGNALPROCESSINH_HPP

class Signal;

namespace tools {
namespace haar 
{
	/*
		
	*/
	void analyse(Signal& s);
	void synthese(Signal& s);
}

namespace biortho97
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}

namespace lifting
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}

Signal& interpolation(Signal& in);
Signal& decimation(Signal& in);
Signal& filtrage(Signal& x, const Signal& h);
}



#endif