#pragma once

#ifndef C_LIFTING_HPP
#define C_LIFTING_HPP

class Signal;
class Signal2D;

namespace lifting
{
	void analyse(Signal& s);
	void synthese(Signal& s);

	void analyse(Signal2D& s);
	void synthese(Signal2D& s);
}

#endif