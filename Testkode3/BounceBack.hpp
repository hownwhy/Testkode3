#pragma once
#include "Globals.hpp"
#include "Dynamics.hpp"
#include "Cell.hpp"


class BounceBack : public Cell{


	

public:
	//// This toNeighbour function does not forward population to the neighbour
	//// Instead it takes the population value and puts it back into the same cell,
	//// but in the oposite direction.
	// void toNeighbour(bool runIndex, Cell cell, int pIndex) override{
	//	cell.setPopulation(!runIndex, reverseDirectionIndex(pIndex), cell.getPolulation(runIndex, pIndex));		
	//}

	~BounceBack() = default;
	BounceBack() = default;

	void collide(const bool runIndexl) override{
		field_t currentPopulation;
		//int reverseDirection;
	
		for (int direction = 0; direction < nDirections; direction++) {			
			currentPopulation = cell.getPolulation(runIndex,(runIndex * nPopulations) + direction);
			
			cell.setPopulation(!runIndex, reverseDirectionIndex(direction), currentPopulation);
		}			
	}
};