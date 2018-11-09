#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include <iostream>
#include <algorithm>
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
	
	//*****************************************************************************************
	// Cell initialization

	void initialize(const bool runIndex, const field_t rho_, const field_t xVelocity, const field_t yVelocity) override {
		rho[runIndex] = 0;
		velocity[runIndex + SpatialDirection::x] = 0;
		velocity[runIndex + SpatialDirection::y] = 0;
		populations.fill(0);
		populationsEq.fill(0);
	}


	void collide(const bool runIndex) override {
		field_t sourceField;

		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			sourceField = getPolulation(runIndex, cellDirection);


			setPopulation(!runIndex, reverseDirectionIndex(cellDirection), sourceField);
		}
	}
	
	void collideAndProppagate(const bool runIndex) {
		field_t sourceField;
		std::shared_ptr<Cell> targetCell;

		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			sourceField = getPolulation(runIndex, cellDirection);
			targetCell = neighbours.getNeighbour(cellDirection);
			targetCell->setPopulation(!runIndex, reverseDirectionIndex(cellDirection), sourceField);
		}
	}

	//void printClassType() override{
	//	 std::cout << "SolidCell" << std::endl;
	//}

	char getCellTypeChar() const override {
		return 'S';
	}

	CellType getCellType() const override{
		return CellType::solidCell;
	}

};