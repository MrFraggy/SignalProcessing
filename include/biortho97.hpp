#pragma once

#ifndef C_BIORTHO97_HPP
#define C_BIORTHO97_HPP

class Signal;
class Signal2D;

namespace biortho97
{
	void analyse(Signal& s);
	void synthese(Signal& s);

	void analyse(Signal2D& s);
	void synthese(Signal2D& s);
}

#endif