#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include <iostream>
//#include <string>
//#include "BounceBack.hpp"
//#include "Dynamics.hpp"


//template <class DYNAMICS>
class SolidCell : public Cell
{
private:
	//DYNAMICS dynamics;

public:
	
	~SolidCell() = default;
	SolidCell() = default;
	
	void collide(const bool runIndex) override {
		field_t sourceField;

		for (int direction = 0; direction < nDirections; direction++) {
			sourceField = getPolulation(runIndex, + direction);

			setPopulation(!runIndex, reverseDirectionIndex(direction), sourceField);
		}
	}

	void printClassType() override{
		 std::cout << "SolidCell" << std::endl;
	}

	char getCellTypeChar() override {
		return 'S';
	}

	CellType getCellType() override{
		return CellType::solidCell;
	}

};