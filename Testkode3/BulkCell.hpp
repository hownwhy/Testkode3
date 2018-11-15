#pragma once
#include "Cell.hpp"
#include <iostream>

class BulkCell : public Cell 
{
public:
	~BulkCell() = default;
	BulkCell() = default;


	//*****************************************************************************************
	// Cell initialization

	void initialize(const bool runIndex, const field_t rho_, const field_t xVelocity, const field_t yVelocity) override {
		rho[runIndex] = rho_;
		velocity[runIndex + SpatialDirection::x] = xVelocity;
		velocity[runIndex + SpatialDirection::y] = yVelocity;
		computePopulationsEq(runIndex);
		std::copy(populationsEq.begin(), populationsEq.end(), populations.begin());
	}	

	// TODO: Use other relaxation times
	void collide(const bool runIndex){
		const int dt = 1;
		const field_t tau = 1;
		computeRho(runIndex);
		computeVelocity(runIndex);
		computePopulationsEq(runIndex);
		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			// Imediate relaxation f[i] = f_eq[i]
			populations[getArrayIndex(!runIndex, cellDirection)] = populationsEq[getArrayIndex(runIndex, cellDirection)];

			/*populations[getArrayIndex(!runIndex, cellDirection)]
				= populations[getArrayIndex(runIndex, cellDirection)] - dt * (populations[getArrayIndex(runIndex, cellDirection)] - populationsEq[getArrayIndex(runIndex, cellDirection)]) / tau;*/
		}
	}

	// The only real difference of collideAndPropagate compared with collide is where the result is stored:
	void collideAndPropagate(const bool runIndex) override{
		const int dt = 1;
		const field_t tau = 1;
		computeRho(runIndex);
		computeVelocity(runIndex);
		computePopulationsEq(runIndex);
		field_t currentPopulation;
		std::shared_ptr<Cell> targetCell;

		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {			
			int arrayIndex = getArrayIndex(runIndex, cellDirection);
			assert(("collideAndPropagate: arrayIndex is negaive:" , arrayIndex >= 0));
			assert(("collideAndPropagate: arrayIndex to high", arrayIndex < nFieldDuplicates * nPopulations));
			// Imediate relaxation f[i] = f_eq[i]
			currentPopulation = populationsEq[arrayIndex];
			targetCell = neighbours.getNeighbour(cellDirection);			
			targetCell->setPopulation(!runIndex, cellDirection, currentPopulation);

			/*populations[getArrayIndex(!runIndex, cellDirection)]
				= populations[getArrayIndex(runIndex, cellDirection)] - dt * (populations[getArrayIndex(runIndex, cellDirection)] - populationsEq[getArrayIndex(runIndex, cellDirection)]) / tau;*/
		}
	}
	
	char getCellTypeChar() const override{
		return 'B';
	}
	
	CellType getCellType() const override {
		return CellType::bulkCell;
	}
};