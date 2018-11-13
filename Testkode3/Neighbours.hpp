#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include <array>
#include <memory>
#include <iostream>

class Cell;

class Neighbours {

private:
	static const int nNeighbours = 8;
	std::array<std::shared_ptr<Cell>, nNeighbours> neighbours;

public:
	void setNeighbour(const int neighbourIndex, const std::shared_ptr<Cell> cell) {		
		//std::cout << "setNeighbour ARRAY_INDEX : " << neighbourIndex << std::endl;
		assert(("setNeighbour: arrayIndex is negative", neighbourIndex >= 0));
		assert(("setNeighbour: arrayIndex to high", neighbourIndex < nNeighbours));
		neighbours[neighbourIndex] = cell;
	}	

	std::shared_ptr<Cell> getNeighbour(const int direction) const {
		//std::cout << "getNeighbour ARRAY_INDEX : " << direction << std::endl;
		assert(("getNeighbour: arrayIndex is negative", direction >= 0));
		assert(("getNeighbour: arrayIndex to high", direction < nNeighbours));
		return neighbours[direction];
	}

	/*template<typename CellType>
	void initializeNeighbour(const int neighbourIndex) {
		neighbours[neighbourIndex] = std::make_shared<CellType>();
	}*/
};