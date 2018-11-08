#pragma once
#include "Globals.hpp"
#include "Neighbours.hpp"
#include <iostream>
#include <array>
#include <memory>


enum CellType : int
{
	cell = 0,
	bulkCell = 1,
	solidCell = 2
};

enum FieldDirection : int 
{
	x = 0,
	y = 1
};

static const int nPopulations = 9;
static const int nFieldDuplicates = 2;
static const int nDimensions = 2;

inline int getArrayIndex(bool runIndex, int fieldIndex) {
	return (runIndex * nPopulations) + fieldIndex;
}


//template<class DYNAMICS>
class Cell {


protected:
	Neighbours neighbours;
	std::array<field_t, nFieldDuplicates * nPopulations> populations;
	std::array<field_t, nFieldDuplicates * nPopulations> populationsEq;
	std::array<field_t, nFieldDuplicates> rho;
	std::array<field_t, nFieldDuplicates * nDimensions> velocity;



public:

	~Cell() = default;
	Cell() = default;
	

	//*****************************************************************************************
	// Do functions

	void propageteTo(const bool runIndex) {
		field_t sourceField;
		std::shared_ptr<Cell> targetCell;

		for (int direction = 0; direction < nDirections; direction++) {
			sourceField = populations[getArrayIndex(runIndex, direction)];
			targetCell = neighbours.getNeighbour(direction);
			targetCell->setPopulation(!runIndex, direction, sourceField);
		}
	}

	virtual void collide(const bool runIndex) = 0;

	void computeRho(const bool runIndex) {
		rho[runIndex] =
			populations[getArrayIndex(runIndex, Direction::east)] +
			populations[getArrayIndex(runIndex, Direction::northEast)] +
			populations[getArrayIndex(runIndex, Direction::north)] +
			populations[getArrayIndex(runIndex, Direction::northWest)] +
			populations[getArrayIndex(runIndex, Direction::west)] +
			populations[getArrayIndex(runIndex, Direction::southWest)] +
			populations[getArrayIndex(runIndex, Direction::south)] +
			populations[getArrayIndex(runIndex, Direction::southEast)] +
			populations[getArrayIndex(runIndex, Direction::rest)];			
	}

	void computeVelocity(const bool runIndex) {
		velocity[getArrayIndex(runIndex, FieldDirection::x)] =
			(populations[getArrayIndex(runIndex, Direction::east)] +
				populations[getArrayIndex(runIndex, Direction::northEast)] +
				populations[getArrayIndex(runIndex, Direction::southEast)] -
				populations[getArrayIndex(runIndex, Direction::west)] -
				populations[getArrayIndex(runIndex, Direction::northWest)] -
				populations[getArrayIndex(runIndex, Direction::southWest)]) / rho[runIndex];

		velocity[getArrayIndex(runIndex, FieldDirection::y)] =
			(populations[getArrayIndex(runIndex, Direction::north)] +
				populations[getArrayIndex(runIndex, Direction::northEast)] +
				populations[getArrayIndex(runIndex, Direction::northWest)] -
				populations[getArrayIndex(runIndex, Direction::south)] -
				populations[getArrayIndex(runIndex, Direction::southEast)] -
				populations[getArrayIndex(runIndex, Direction::southWest)]) / rho[runIndex];
	}
	
	void computePopulationsEq(const bool runIndex) {
		field_t ux = velocity[getArrayIndex(runIndex, FieldDirection::x)];
		field_t uy = velocity[getArrayIndex(runIndex, FieldDirection::y)];
		field_t uxSqr = ux * ux;
		field_t uySqr = uy * uy;
		field_t uxuy = ux * uy;
		field_t uSqr = sqrt(uxSqr + uySqr);

		// Weights
		field_t weightR	= (2 * rho[runIndex]) / 2;	// Rest
		field_t weightHV	= rho[runIndex] / 18;	// Horizontal/Vertical
		field_t weightD	= rho[runIndex] / 36;		// Diagonal

		// Calculate the rest equlibrium field component
		populationsEq[Direction::rest] = weightR * (2 - (3 * uSqr));

		// Calculate horizontal and vertical equlibrium field components
		populationsEq[Direction::east]	= weightHV * (2 + (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[Direction::north]	= weightHV * (2 + (6 * uy) + (9 * uySqr) - (3 * uSqr));
		populationsEq[Direction::west]	= weightHV * (2 - (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[Direction::south] = weightHV * (2 - (6 * uy) + (9 * uySqr) - (3 * uSqr));

		// Calculate diagonal equlibrium field components
		populationsEq[Direction::northEast] = weightD * (1 + (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[Direction::northWest] = weightD * (1 - (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
		populationsEq[Direction::southWest] = weightD * (1 - (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[Direction::southEast] = weightD * (1 + (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
	}


	//*****************************************************************************************
	// Set functions
	void setPopulation(const bool runIndex, const int populationIndex, const field_t fieldValue) {
		populations[getArrayIndex(runIndex, populationIndex)] = fieldValue;
	}
	void setRho(const bool runIndex,const field_t rho_) {
		rho[runIndex] = rho_;
	}
	

	//*****************************************************************************************
// Get functions

	Neighbours &getCellNeighbours() {
		return neighbours;
	}
	const field_t getPolulation(const bool runIndex, const int populationIndex) const {
		return populations[getArrayIndex(runIndex, populationIndex)];
	}
	const field_t getRho(const bool runIndex) const {
		return rho[runIndex];
	}
	virtual char getCellTypeChar() {
		return 'C';
	}

	virtual CellType getCellType() {
		return CellType::cell;
	}

	//virtual void printClassType() {
	//	std::cout << "Cell" << std::endl;
	//}
};