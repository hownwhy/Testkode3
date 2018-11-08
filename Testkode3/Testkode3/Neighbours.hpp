#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include <array>
#include <memory>

class Cell;

class Neighbours {

private:
	static const int nNeighbours = 8;
	std::array<std::shared_ptr<Cell>, nNeighbours> neighbours;

public:
	void setNeighbour(const int neighbourIndex, std::shared_ptr<Cell> cell) {
		neighbours[neighbourIndex] = cell;
	}	

	std::shared_ptr<Cell> getNeighbour(int direction) const {
		return neighbours[direction];
	}

	template<typename CellType>
	void initializeNeighbour(const int neighbourIndex) {
		neighbours[neighbourIndex] = std::make_shared<CellType>();
	}
};