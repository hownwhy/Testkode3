#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include "BulkCell.hpp"
#include "SolidCell.hpp"
#include <memory>



class Grid {


private:
	static const int xDim = 5;
	static const int yDim = 5;

	std::array<int, xDim * yDim> geometry;
	std::array<std::shared_ptr<Cell>, xDim * yDim> grid;


	inline int gridPosition(int x_, int y_) {
		return (y_ * xDim) + x_;
	}


	int gridNeigbourPossition(int x, int y, int direction) {

		int dx = 0;
		int dy = 0;

		switch (direction) {
		case Direction::east:		dx = +1;
			break;
		case Direction::northEast:	dx = +1;
			dy = -1;
			break;
		case Direction::north:		dy = -1;
			break;
		case Direction::northWest:	dx = -1;
			dy = -1;
			break;
		case Direction::west:		dx = -1;
			break;
		case Direction::southWest:	dx = -1;
			dy = +1;
			break;
		case Direction::south:		dy = +1;
			break;
		case Direction::southEast:	dx = +1;
									dy = +1;
			break;
		case Direction::rest:
		default:					dx = 0;
									dy = 0;
		}

		return gridPosition(x + dx, y + dy);
	}


public:

	void makeGeometry() {
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (y <= 1 || y >= yDim - 2 || x <= 1 || x >= xDim - 2) {
					geometry[(y * xDim) + x] = 1;
				}
				else {
					geometry[(y * xDim) + x] = 0;
				}
			}
		}
	}


	void makeGrid() {

		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (geometry[gridPosition(x, y)] == 0) {
					grid[gridPosition(x, y)] = std::make_shared<BulkCell>();
					for (int i = 0; i < nFieldDuplicates; i++) {
						for (int p = 0; p < nPopulations; p++) {
							grid[gridPosition(x, y)]->setPopulation(i, p, 9);
						}
					}
				}
				else if (geometry[gridPosition(x, y)] == 1) {
					grid[gridPosition(x, y)] = std::make_shared<SolidCell>();
					for (int i = 0; i < nFieldDuplicates; i++) {
						for (int p = 0; p < nPopulations; p++) {
							grid[gridPosition(x, y)]->setPopulation(i, p, 8);
						}
					}
				}
			}
		}
	}

	void linkNeighbours() {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim- xMargin; x++) {				
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::east,			grid[gridNeigbourPossition(x, y, Direction::east)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::northEast,	grid[gridNeigbourPossition(x, y, Direction::northEast)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::north,		grid[gridNeigbourPossition(x, y, Direction::north)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::northWest,	grid[gridNeigbourPossition(x, y, Direction::northWest)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::west,			grid[gridNeigbourPossition(x, y, Direction::west)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::southWest,	grid[gridNeigbourPossition(x, y, Direction::southWest)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::south,		grid[gridNeigbourPossition(x, y, Direction::south)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(Direction::southEast,	grid[gridNeigbourPossition(x, y, Direction::southEast)]);				
			}
		}
	}


	//******************************************************************************************************************
	//******************************************************************************************************************
	// Do routines
	//******************************************************************************************************************

	void propagate(const bool runIndex) {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				grid[gridPosition(x, y)]->propageteTo(runIndex);
			}
		}
	}

	void collide(const bool runIndex) {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				grid[gridPosition(x, y)]->collide(runIndex);
			}
		}
	}



	
	//******************************************************************************************************************
	//******************************************************************************************************************
	// Print routines
	//******************************************************************************************************************

	void printGeometry() {
		std::cout << std::endl;
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				std::cout << geometry[(y * xDim) + x];
			}
			std::cout << std::endl;
		}
	}

	//******************************************************************************************************************
	// Cell type - print routines
	void printCellType() {
		std::cout << std::endl;
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (grid[gridPosition(x, y)] != nullptr) {
					std::cout << grid[(y * xDim) + x]->getCellTypeChar();
					//std::cout << " - ";
				}
				else {
					std::cout << "N";
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}


	//******************************************************************************************************************
	// Neighbours - print routines
	void printNeighboursCellType(int x, int y) {
		std::shared_ptr<Cell> tempCell;
		char E, NE, N, NW, W, SW, S, SE, R;

		//std::cout << std::endl;

		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::east)) != nullptr) {
			E = tempCell->getCellTypeChar();
		}
		else {
			E = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::northEast)) != nullptr) {
			NE = tempCell->getCellTypeChar();
		}
		else {
			NE = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::north)) != nullptr) {
			N = tempCell->getCellTypeChar();
		}
		else {
			N = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::northWest)) != nullptr) {
			NW = tempCell->getCellTypeChar();
		}
		else {
			NW = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::west)) != nullptr) {
			W = tempCell->getCellTypeChar();
		}
		else {
			W = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::southWest)) != nullptr) {
			SW = tempCell->getCellTypeChar();
		}
		else {
			SW = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::south)) != nullptr) {
			S = tempCell->getCellTypeChar();
		}
		else {
			S = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(Direction::southEast)) != nullptr) {
			SE = tempCell->getCellTypeChar();
		}
		else {
			SE = 'N';
		}

		// Rest shows cell at (x, y)
		if ((tempCell = grid[gridPosition(x, y)]) != nullptr) {
			R = tempCell->getCellTypeChar();
		}
		else {
			R = 'N';
		}

		std::cout << std::endl;
		std::cout << NW
			<< " " << N
			<< " " << NE << std::endl;

		std::cout << " \\|/ " << std::endl;

		std::cout << W
			<< "-" << R
			<< "-" << E << std::endl;

		std::cout << " /|\\ " << std::endl;

		std::cout << SW
			<< " " << S
			<< " " << SE << std::endl;
	}

	void printNeighboursCellType() {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				printNeighboursCellType(x, y);
			}
		}
	}


	//******************************************************************************************************************
	// Cell Population - print routines

	void printCellPopulation(bool runIndex, int x, int y) {
		std::cout << std::endl;
		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::northWest)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::north)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::northEast) << std::endl;

		std::cout << " \\|/ " << std::endl;

		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::west)
			<< "-" << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::rest)
			<< "-" << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::east) << std::endl;

		std::cout << " /|\\ " << std::endl;

		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::southWest)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::south)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, Direction::southEast) << std::endl;
	}

	void  printCellPopulation(bool runIndex) {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				printCellPopulation(runIndex, x, y);
			}
		}
	}
};