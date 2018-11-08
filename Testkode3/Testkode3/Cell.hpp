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


//template<class DYNAMICS>
class Cell {


protected:
	Neighbours neighbours;
	std::array<field_t, nFieldDuplicates * nPopulations> population;
	field_t rho;
	field_t velocity;



public:

	~Cell() = default;
	Cell() = default;
	

	//*****************************************************************************************
	// Do functions

	void propageteTo(const bool runIndex) {
		field_t sourceField;
		std::shared_ptr<Cell> targetCell;

		for (int direction = 0; direction < nDirections; direction++) {
			sourceField = population[pIndexGen(runIndex, direction)];
			targetCell = neighbours.getNeighbour(direction);
			targetCell->setPopulation(!runIndex, direction, sourceField);
		}
	}

	virtual void collide(const bool runIndex) = 0;

	virtual void printClassType() {
		std::cout << "Cell" << std::endl;
	}

	virtual char getCellTypeChar() {
		return 'C';
	}

	virtual CellType getCellType() {
		return CellType::cell;
	}


	//*****************************************************************************************
	// Set functions
	void setPopulation(const bool runIndex, const int populationIndex, const field_t fieldValue) {
		population[pIndexGen(runIndex, populationIndex)] = fieldValue;
	}
	void setRho(const field_t rho_) {
		rho = rho_;
	}
	

	//*****************************************************************************************
// Get functions

	Neighbours &getCellNeighbours() {
		return neighbours;
	}
	const field_t getPolulation(const bool runIndex, const int populationIndex) const {
		return population[pIndexGen(runIndex, populationIndex)];
	}
	const field_t getRho() const {
		return rho;
	}
};