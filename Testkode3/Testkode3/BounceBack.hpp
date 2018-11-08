#pragma once
#include "Globals.hpp"
#include "Dynamics.hpp"
#include "Cell.hpp"


class BounceBack : public Dynamics{


	

public:
	//// This toNeighbour function does not forward population to the neighbour
	//// Instead it takes the population value and puts it back into the same cell,
	//// but in the oposite direction.
	// void toNeighbour(bool runIndex, Cell cell, int pIndex) override{
	//	cell.setPopulation(!runIndex, reverseDirectionIndex(pIndex), cell.getPolulation(runIndex, pIndex));		
	//}


	BounceBack() = default;

	void collide(int runIndex, Cell& cell) override{
		field_t sourceField;
		//int reverseDirection;
	
		for (int direction = 0; direction < nDirections; direction++) {			
			sourceField = cell.getPolulation(runIndex,(runIndex * nPopulations) + direction);
			
			cell.setPopulation(!runIndex, reverseDirectionIndex(direction), sourceField);
		}

			
	}
};