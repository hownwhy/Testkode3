#pragma once
#include "Cell.hpp"
#include <iostream>

class BulkCell : public Cell 
{
public:
	~BulkCell() = default;
	BulkCell() = default;
	

	// TODO: Use other relaxation times
	void collide(const bool runIndex){
		computeRho(runIndex);
		computeVelocity(runIndex);
		computePopulationsEq(runIndex);
		for (int p = 0; p < 9; p++) {
			// Imediate relaxation f[i] = f_eq[i]
			populations[getArrayIndex(!runIndex, p)] = populationsEq[getArrayIndex(runIndex, p)];
		}
	}

	//void printClassType() override {
	//	std::cout << "BulkCell" << std::endl;
	//}

	char getCellTypeChar() override {
		return 'B';
	}

	CellType getCellType() override {
		return CellType::bulkCell;
	}
};