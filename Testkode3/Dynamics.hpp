#pragma once
#include "Globals.hpp"
#include "Cell.hpp"

//template<class DYNAMICS> class Cell;

//template <class SPECIFIC_DYNAMICS>

class Dynamics {

public:
	Dynamics() = default;

	virtual void collide(const bool runIndex, Cell& cell) = 0;
};