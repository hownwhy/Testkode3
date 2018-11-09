#pragma once
#include "Globals.hpp"
#include "Cell.hpp"
#include "BulkCell.hpp"
#include "SolidCell.hpp"
#include <memory>



class Grid {


private:
	static const int xDim = 6;
	static const int yDim = 6;

	std::array<int, xDim * yDim> geometry;
	std::array<std::shared_ptr<Cell>, xDim * yDim> grid;


	inline int gridPosition(const int x_, const int y_) const{
		return (y_ * xDim) + x_;
	}


	//******************************************************************************************************************
	//******************************************************************************************************************
	// Prepare and initialize
	//******************************************************************************************************************


	int gridNeigbourPossition(const int x, const int y, const int direction) const {

		int dx = 0;
		int dy = 0;

		switch (direction) {
		case CellDirection::east:		dx = +1;
			break;
		case CellDirection::northEast:	dx = +1;
			dy = -1;
			break;
		case CellDirection::north:		dy = -1;
			break;
		case CellDirection::northWest:	dx = -1;
			dy = -1;
			break;
		case CellDirection::west:		dx = -1;
			break;
		case CellDirection::southWest:	dx = -1;
			dy = +1;
			break;
		case CellDirection::south:		dy = +1;
			break;
		case CellDirection::southEast:	dx = +1;
									dy = +1;
			break;
		case CellDirection::rest:
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
						for (int cellDirection = 0; cellDirection < nPopulations; cellDirection++) {
							grid[gridPosition(x, y)]->setPopulation(i, cellDirection, 9);
						}
					}
				}
				else if (geometry[gridPosition(x, y)] == 1) {
					grid[gridPosition(x, y)] = std::make_shared<SolidCell>();
					for (int i = 0; i < nFieldDuplicates; i++) {
						for (int cellDirection = 0; cellDirection < nPopulations; cellDirection++) {
							grid[gridPosition(x, y)]->setPopulation(i, cellDirection, 8);
						}
					}
				}
			}
		}
	}

	void linkNeighbours() const {
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim- xMargin; x++) {				
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::east,			grid[gridNeigbourPossition(x, y, CellDirection::east)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::northEast,	grid[gridNeigbourPossition(x, y, CellDirection::northEast)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::north,		grid[gridNeigbourPossition(x, y, CellDirection::north)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::northWest,	grid[gridNeigbourPossition(x, y, CellDirection::northWest)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::west,			grid[gridNeigbourPossition(x, y, CellDirection::west)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::southWest,	grid[gridNeigbourPossition(x, y, CellDirection::southWest)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::south,		grid[gridNeigbourPossition(x, y, CellDirection::south)]);
				grid[gridPosition(x, y)]->getCellNeighbours().setNeighbour(CellDirection::southEast,	grid[gridNeigbourPossition(x, y, CellDirection::southEast)]);				
			}
		}
	}

	void setAllRho(const field_t rho) const {
		const int xMargin = 1;
		const int yMargin = 1;		
		for(int runIndex = 0; runIndex < 2; runIndex++){
			for (int y = yMargin; y < yDim - yMargin; y++) {
				for (int x = xMargin; x < xDim - xMargin; x++) {
					grid[gridPosition(x, y)]->setRho(runIndex, rho);
				}
			}
		}
	}

	void gridInitialize(const bool runIndex) const {
		const int xMargin = 1;
		const int yMargin = 1;
		const field_t rho = 1;
		const field_t xVelocity = 0;
		const field_t yVelocity = 0;
		//const 
		for (int runIndex = 0; runIndex < 2; runIndex++) {
			for (int y = yMargin; y < yDim - yMargin; y++) {
				for (int x = xMargin; x < xDim - xMargin; x++) {
					grid[gridPosition(x, y)]->initialize(runIndex, rho, xVelocity, yVelocity);
				}
			}
		}
	}

	//******************************************************************************************************************
	//******************************************************************************************************************
	// Do routines
	//******************************************************************************************************************

	void propagate(const bool runIndex) const{
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				grid[gridPosition(x, y)]->propageteTo(runIndex);
			}
		}
	}

	void collide(const bool runIndex) const{
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				grid[gridPosition(x, y)]->collide(runIndex);
			}
		}
	}

	void computeAllRho(const bool runIndex) const{
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				grid[gridPosition(x, y)]->computeRho(runIndex);
			}
		}
	}



	
	//******************************************************************************************************************
	//******************************************************************************************************************
	// Print routines
	//******************************************************************************************************************

	void printGeometry() const{
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
	void printCellType() const{
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
	void printNeighboursCellType(const int x, const int y) const{
		std::shared_ptr<Cell> tempCell;
		char E, NE, N, NW, W, SW, S, SE, R;

		//std::cout << std::endl;

		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::east)) != nullptr) {
			E = tempCell->getCellTypeChar();
		}
		else {
			E = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::northEast)) != nullptr) {
			NE = tempCell->getCellTypeChar();
		}
		else {
			NE = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::north)) != nullptr) {
			N = tempCell->getCellTypeChar();
		}
		else {
			N = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::northWest)) != nullptr) {
			NW = tempCell->getCellTypeChar();
		}
		else {
			NW = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::west)) != nullptr) {
			W = tempCell->getCellTypeChar();
		}
		else {
			W = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::southWest)) != nullptr) {
			SW = tempCell->getCellTypeChar();
		}
		else {
			SW = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::south)) != nullptr) {
			S = tempCell->getCellTypeChar();
		}
		else {
			S = 'N';
		}
		if ((tempCell = grid[gridPosition(x, y)]->getCellNeighbours().getNeighbour(CellDirection::southEast)) != nullptr) {
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

	void printNeighboursCellType() const {
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

	void printCellPopulation(const bool runIndex, const int x, const int y) const{
		std::cout << std::endl;
		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::northWest)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::north)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::northEast) << std::endl;

		std::cout << " \\|/ " << std::endl;

		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::west)
			<< "-" << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::rest)
			<< "-" << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::east) << std::endl;

		std::cout << " /|\\ " << std::endl;

		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::southWest)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::south)
			<< " " << grid[gridPosition(x, y)]->getPolulation(runIndex, CellDirection::southEast) << std::endl;
	}

	void  printCellPopulation(const bool runIndex) const{
		const int xMargin = 1;
		const int yMargin = 1;
		for (int y = yMargin; y < yDim - yMargin; y++) {
			for (int x = xMargin; x < xDim - xMargin; x++) {
				printCellPopulation(runIndex, x, y);
			}
		}
	}

	//******************************************************************************************************************
	// Cell rho - print routines
	void printCellRho(const bool runIndex) const{
		std::cout << std::endl;
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (grid[gridPosition(x, y)] != nullptr) {
					std::cout << grid[gridPosition(x, y)]->getRho(runIndex);
					std::cout << " - ";
				}
			}
			std::cout << std::endl;
		}
	}

	//******************************************************************************************************************
	// Cell velocity - print routines
	void printCellVelocity(const bool runIndex) const{
		std::cout << std::endl;
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (grid[gridPosition(x, y)] != nullptr) {
					std::cout << "(" << grid[gridPosition(x, y)]->getVelocity(runIndex, SpatialDirection::x) 
						<< "," 
						<< grid[gridPosition(x, y)]->getVelocity(runIndex, SpatialDirection::y) << ")";
					std::cout << " - ";
				}
			}
			std::cout << std::endl;
		}
	}
};