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

	void collideAndProppagate(const bool runIndex) override{
		const int dt = 1;
		const field_t tau = 1;
		computeRho(runIndex);
		computeVelocity(runIndex);
		computePopulationsEq(runIndex);

		field_t sourceField;
		std::shared_ptr<Cell> targetCell;

		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {
			// Imediate relaxation f[i] = f_eq[i]
			int arrayIndex = getArrayIndex(runIndex, cellDirection);
			assert(("collideAndProppagate: arrayIndex is negaive:" , arrayIndex >= 0));
			assert(("collideAndProppagate: arrayIndex to high", arrayIndex < nFieldDuplicates * nPopulations));
			sourceField = populationsEq[arrayIndex];
			//sourceField = populations[arrayIndex];
			targetCell = neighbours.getNeighbour(cellDirection);
			
			
			targetCell->setPopulation(!runIndex, cellDirection, sourceField);
			/*populations[getArrayIndex(!runIndex, cellDirection)]
				= populations[getArrayIndex(runIndex, cellDirection)] - dt * (populations[getArrayIndex(runIndex, cellDirection)] - populationsEq[getArrayIndex(runIndex, cellDirection)]) / tau;*/
		}
	}

	//void printClassType() const override {
	//	std::cout << "BulkCell" << std::endl;
	//}

	char getCellTypeChar() const override{
		return 'B';
	}

	CellType getCellType() const override {
		return CellType::bulkCell;
	}
};