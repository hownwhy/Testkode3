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

enum SpatialDirection : int 
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
	// Cell initialization

	virtual void initialize(const bool runIndex, const field_t rho_, const field_t xVelocity, const field_t yVelocity) = 0;


	//*****************************************************************************************
	// Do functions

	void propageteTo(const bool runIndex) const {
		field_t sourceField;
		std::shared_ptr<Cell> targetCell;

		for (int direction = 0; direction < nDirections; direction++) {
			sourceField = populations[getArrayIndex(runIndex, direction)];
			targetCell = neighbours.getNeighbour(direction);
			targetCell->setPopulation(!runIndex, direction, sourceField);
		}
	}

	virtual void collide(const bool runIndex) = 0;

	virtual void collideAndProppagate(const bool runIndex) = 0;

	void computeRho(const bool runIndex) {
		rho[runIndex] =
			populations[getArrayIndex(runIndex, CellDirection::east)] +
			populations[getArrayIndex(runIndex, CellDirection::northEast)] +
			populations[getArrayIndex(runIndex, CellDirection::north)] +
			populations[getArrayIndex(runIndex, CellDirection::northWest)] +
			populations[getArrayIndex(runIndex, CellDirection::west)] +
			populations[getArrayIndex(runIndex, CellDirection::southWest)] +
			populations[getArrayIndex(runIndex, CellDirection::south)] +
			populations[getArrayIndex(runIndex, CellDirection::southEast)] +
			populations[getArrayIndex(runIndex, CellDirection::rest)];			
	}

	void computeVelocity(const bool runIndex) {
		velocity[runIndex + SpatialDirection::x] =
			(populations[getArrayIndex(runIndex, CellDirection::east)] +
				populations[getArrayIndex(runIndex, CellDirection::northEast)] +
				populations[getArrayIndex(runIndex, CellDirection::southEast)] -
				populations[getArrayIndex(runIndex, CellDirection::west)] -
				populations[getArrayIndex(runIndex, CellDirection::northWest)] -
				populations[getArrayIndex(runIndex, CellDirection::southWest)]) / rho[runIndex];

		velocity[runIndex + SpatialDirection::y] =
			(populations[getArrayIndex(runIndex, CellDirection::north)] +
				populations[getArrayIndex(runIndex, CellDirection::northEast)] +
				populations[getArrayIndex(runIndex, CellDirection::northWest)] -
				populations[getArrayIndex(runIndex, CellDirection::south)] -
				populations[getArrayIndex(runIndex, CellDirection::southEast)] -
				populations[getArrayIndex(runIndex, CellDirection::southWest)]) / rho[runIndex];
	}
	
	void computePopulationsEq(const bool runIndex) {
		field_t ux = velocity[runIndex + SpatialDirection::x];
		field_t uy = velocity[runIndex + SpatialDirection::x];
		field_t uxSqr = ux * ux;
		field_t uySqr = uy * uy;
		field_t uxuy = ux * uy;
		field_t uSqr = uxSqr + uySqr;

		// Weights
		field_t weightR	= (2 * rho[runIndex]) / 9;	// Rest
		field_t weightHV	= rho[runIndex] / 18;	// Horizontal/Vertical
		field_t weightD	= rho[runIndex] / 36;		// Diagonal

		// Calculate the rest equlibrium field component
		populationsEq[CellDirection::rest] = weightR * (2 - (3 * uSqr));

		// Calculate horizontal and vertical equlibrium field components
		populationsEq[CellDirection::east]	= weightHV * (2 + (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[CellDirection::north]	= weightHV * (2 + (6 * uy) + (9 * uySqr) - (3 * uSqr));
		populationsEq[CellDirection::west]	= weightHV * (2 - (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[CellDirection::south] = weightHV * (2 - (6 * uy) + (9 * uySqr) - (3 * uSqr));

		// Calculate diagonal equlibrium field components
		populationsEq[CellDirection::northEast] = weightD * (1 + (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[CellDirection::northWest] = weightD * (1 - (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
		populationsEq[CellDirection::southWest] = weightD * (1 - (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[CellDirection::southEast] = weightD * (1 + (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
	}


	//*****************************************************************************************
	// Set functions
	void setPopulation(const bool runIndex, const int populationIndex, const field_t fieldValue) {
		populations[getArrayIndex(runIndex, populationIndex)] = fieldValue;
	}
	void setRho(const bool runIndex, const field_t rho_) {
		rho[runIndex] = rho_;
	}
	void setVelocity(const bool runIndex, const field_t velocity_) {
		velocity[runIndex] = velocity_;
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
	const field_t getVelocity(const bool runIndex, const SpatialDirection direction) const{
		return velocity[runIndex + direction];
	}

	virtual char getCellTypeChar() const{
		return 'C';
	}

	virtual CellType getCellType() const{
		return CellType::cell;
	}

	//virtual void printClassType() const{
	//	std::cout << "Cell" << std::endl;
	//}
};