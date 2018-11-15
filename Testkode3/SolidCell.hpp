#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include <iostream>
#include <algorithm>


class SolidCell : public Cell
{
private:
	

public:
	
	~SolidCell() = default;
	SolidCell() = default;

	
	//*****************************************************************************************
	// Cell initialization

	void initialize(const bool runIndex, const field_t rho_, const field_t xVelocity, const field_t yVelocity) override {
		rho[runIndex] = rho_;
		velocity[runIndex + SpatialDirection::x] = xVelocity;
		velocity[runIndex + SpatialDirection::y] = yVelocity;
		computePopulationsEq(runIndex);
		std::copy(populationsEq.begin(), populationsEq.end(), populations.begin());
	}

	// BounceBack
	// The current population of this run is reversed and made available for the next run.
	void collide(const bool runIndex) override {
		field_t currentPopulation;
		
		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			currentPopulation = getPolulation(runIndex, cellDirection);
			setPopulation(!runIndex, reverseDirectionIndex(cellDirection), currentPopulation);
		}
	}

	// BounceBack and proppagate
	// The current population of this run is reversed and made available in the neighbour cell for the next run.
	void collideAndPropagate(const bool runIndex) {
		field_t currentPopulation;
		std::shared_ptr<Cell> targetCell;

		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			currentPopulation = getPolulation(runIndex, cellDirection);
			targetCell = neighbours.getNeighbour(cellDirection);
			targetCell->setPopulation(!runIndex, reverseDirectionIndex(cellDirection), currentPopulation);
		}
	}

	char getCellTypeChar() const override {
		return 'S';
	}
	
	CellType getCellType() const override{
		return CellType::solidCell;
	}
};