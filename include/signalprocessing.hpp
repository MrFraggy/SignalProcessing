#pragma once

#ifndef C_SIGNALPROCESSING_HPP
#define C_SIGNALPROCESSINH_HPP

class Signal;

namespace haar 
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}

namespace biortho97
{
	void analyse(Signal& s);
	void synthese(Signal& s);
}




#endif