#pragma once
#include "Cell.hpp"
#include <iostream>

class BulkCell : public Cell 
{
public:
	~BulkCell() = default;
	BulkCell() = default;
	/*void propageteTo(bool runIndex) override {
		for (int direction = 0; direction < nDirections; direction++) {
			neighbours.getNeighbour(direction)->setPopulation(!runIndex, direction, population[pIndexGen(runIndex, direction)]);

		}
	}*/

	void collide(const bool runIndex){}

	void printClassType() override {
		std::cout << "BulkCell" << std::endl;
	}

	char getCellTypeChar() override {
		return 'B';
	}

	CellType getCellType() override {
		return CellType::bulkCell;
	}
};