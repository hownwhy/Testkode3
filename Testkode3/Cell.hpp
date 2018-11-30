#pragma once
#include "Globals.hpp"
#include "Neighbours.hpp"
#include <iostream>
#include <string>
#include <array>
#include <memory>


const enum CellType : int
{
	cell = 0,
	bulkCell = 1,
	solidCell = 2,
	periodicCell = 3
};

enum SpatialDirection : int 
{
	x = 0,
	y = 1
};

// TODO: Where should these static constants be? Here it will be accessible to all who include the hpp file.
// If declareedd as a member, it has to be initialized in a cpp file...
static const int nPopulations = 9;		// Number of poulations for species
static const int nFieldDuplicates = 2;	// Number of field "duplicats" (for temporary storage)(probably no more than 2)
static const int nDimensions = 2;		
static const int dt = 1;
static const field_t tau = 0.6;
static const std::array<field_t, 2> force = { 0.01, 0 };


// TODO: See what can be done with templates instead of virtual functions
class Cell {

protected:
	Neighbours neighbours;
	std::array<field_t, nFieldDuplicates * nPopulations> populations;
	std::array<field_t, nFieldDuplicates * nPopulations> populationsEq;
	std::array<field_t, nFieldDuplicates> rho;
	std::array<field_t, nFieldDuplicates * nDimensions> velocity;
	std::array<field_t, nPopulations> sourceTerms;
	

public:

	virtual ~Cell() = default;
	Cell() = default;
	
	// Returns the 1D array index which depend on the 2D (runIndex, direction).
	static inline int getArrayIndex(bool runIndex, int direction) {
		return (runIndex * nPopulations) + direction;
	}
	//*****************************************************************************************
	// Cell initialization

	void initialize(const bool runIndex, const field_t rho_, const field_t xVelocity, const field_t yVelocity) {
		rho[runIndex] = rho_;
		velocity[runIndex + SpatialDirection::x] = xVelocity;
		velocity[runIndex + SpatialDirection::y] = yVelocity;
		computePopulationsEq(runIndex);
		std::copy(populationsEq.begin() + (runIndex * nPopulations), populationsEq.end() - (!runIndex * nPopulations), populations.begin() + (runIndex * nPopulations));
	}

	void initializeRho(const bool runIndex, const field_t rho_) {
		rho[runIndex] = rho_;
		computePopulationsEq(runIndex);
		//std::copy(populationsEq.at(runIndex * nPopulations), populationsEq.at((runIndex * nPopulations) + nPopulations), populations.at(runIndex * nPopulations));
		std::copy(populationsEq.begin()+(runIndex * nPopulations), populationsEq.end() - (!runIndex * nPopulations), populations.begin()+(runIndex * nPopulations));
	}

	void initializeVelocity(const bool runIndex, const SpatialDirection direction, const field_t velocity_) {
		velocity[runIndex * nDimensions + direction] = velocity_;
		computePopulationsEq(runIndex);
		//std::copy(populationsEq.at(runIndex * nPopulations), populationsEq.at((runIndex * nPopulations) + nPopulations), populations.at(runIndex * nPopulations));
		std::copy(populationsEq.begin() + (runIndex * nPopulations), populationsEq.end() - (!runIndex * nPopulations), populations.begin() + (runIndex * nPopulations));
	}

	//*****************************************************************************************
	// Do functions

	// Propagate populations to neighbouring cells (notice the: !runIndex) used
	// so that it will not overwrite populations that will be used in the current run (loop).	
	// TODO: The local variables used for readability may (or may not) decrease performance. Find out. 
	void propageteTo(const bool runIndex) const {
		field_t currentPopulation;
		std::shared_ptr<Cell> targetCell;

		for (int direction = 0; direction < nDirections; direction++) {
			currentPopulation = populations[getArrayIndex(runIndex, direction)];
			targetCell = neighbours.getNeighbour(direction);
			targetCell->setReceived(!runIndex, direction, currentPopulation);
		}
	}

	virtual void setReceived(const bool runIndex, const int populationIndex, const field_t fieldValue) = 0;

	// TODO: Use other relaxation times
	// TODO: put correct source term in call to "comouteVelocity"
	void collide(const bool runIndex) {
		//std::cout << "collide" << std::endl;
		computeRho(runIndex);
		computeVelocity(runIndex, force);
		computePopulationsEq(runIndex);
		for (int cellDirection = 0; cellDirection < nDirections; cellDirection++) {			
			populations[getArrayIndex(runIndex, cellDirection)]
				= populations[getArrayIndex(runIndex, cellDirection)] - dt * (populations[getArrayIndex(runIndex, cellDirection)] - populationsEq[getArrayIndex(runIndex, cellDirection)]) / tau;
			populations[getArrayIndex(runIndex, cellDirection)] += sourceTerms[cellDirection];
		}
	}

	virtual void collideAndPropagate(const bool runIndex) = 0;

	void computeSourceTerm(const int runIndex) {
		static const field_t weightR = 4 / 9;
		static const field_t weightHV = 1 / 9;
		static const field_t weightD = 1 / 36;
		static const field_t dtTauFactorR = 2 * (dt - 2 * tau) / (3 * tau);
		static const field_t dtTauFactorHV = (dt - 2 * tau) / (6 * tau);
		static const field_t dtTauFactorD = (dt - 2 * tau) / (24 * tau);
		static const std::array<field_t, 2> force;
		static const field_t Fx = force[SpatialDirection::x];
		static const field_t Fy = force[SpatialDirection::y];
		static const field_t ux = velocity[SpatialDirection::x];
		static const field_t uy = velocity[SpatialDirection::y];
		static const field_t FxUx = Fx * ux;
		static const field_t FxUy = Fx * uy;
		static const field_t FyUy = Fy * uy;
		static const field_t FyUx = Fy * ux;


		for (int populationIndex = 0; populationIndex < nPopulations; populationIndex++) {

			// Calculate the rest source field component
			sourceTerms[CellDirection::rest] = dtTauFactorR * (FxUx + FyUy);

			// Calculate horizontal and vertical source field components
			sourceTerms[CellDirection::east] = dtTauFactorHV * (FyUy - 2 * FxUx - Fx);
			sourceTerms[CellDirection::north] = dtTauFactorHV * (FxUx - 2 * FyUy - Fy);
			sourceTerms[CellDirection::west] = dtTauFactorHV * (FyUy - 2 * FxUx + Fx);
			sourceTerms[CellDirection::south] = dtTauFactorHV * (FxUx - 2 * FyUy + Fy);

			// Calculate diagonal source field components
			sourceTerms[CellDirection::northEast] = -dtTauFactorD * (Fx + Fy + 2 * FxUx + 3 * FyUx + 3 * FxUy + 2 * FyUy);
			sourceTerms[CellDirection::northWest] = -dtTauFactorD * (-Fx + Fy + 2 * FxUx - 3 * FyUx - 3 * FxUy + 2 * FyUy);
			sourceTerms[CellDirection::southWest] = -dtTauFactorD * (-Fx - Fy + 2 * FxUx + 3 * FyUx + 3 * FxUy + 2 * FyUy);
			sourceTerms[CellDirection::southEast] = -dtTauFactorD * (Fx - Fy + 2 * FxUx - 3 * FyUx - 3 * FxUy + 2 * FyUy);

		}
	}

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

	void computeVelocity(const bool runIndex, const std::array<field_t, 2> bodyForce) {
		velocity[runIndex * nDimensions + SpatialDirection::x] =
			((populations[getArrayIndex(runIndex, CellDirection::east)] +
				populations[getArrayIndex(runIndex, CellDirection::northEast)] +
				populations[getArrayIndex(runIndex, CellDirection::southEast)] -
				populations[getArrayIndex(runIndex, CellDirection::west)] -
				populations[getArrayIndex(runIndex, CellDirection::northWest)] -
				populations[getArrayIndex(runIndex, CellDirection::southWest)]) / rho[runIndex]) + bodyForce[SpatialDirection::x]/(2 * rho[runIndex]);

		velocity[runIndex * nDimensions + SpatialDirection::y] =
			((populations[getArrayIndex(runIndex, CellDirection::north)] +
				populations[getArrayIndex(runIndex, CellDirection::northEast)] +
				populations[getArrayIndex(runIndex, CellDirection::northWest)] -
				populations[getArrayIndex(runIndex, CellDirection::south)] -
				populations[getArrayIndex(runIndex, CellDirection::southEast)] -
				populations[getArrayIndex(runIndex, CellDirection::southWest)]) / rho[runIndex]) + bodyForce[SpatialDirection::y] / (2 * rho[runIndex]);
	}	
	
	void computePopulationsEq(const bool runIndex) {
		field_t ux = velocity[runIndex * nDimensions + SpatialDirection::x];
		field_t uy = velocity[runIndex * nDimensions + SpatialDirection::y];
		field_t uxSqr = ux * ux;
		field_t uySqr = uy * uy;
		field_t uxuy = ux * uy;
		field_t uSqr = uxSqr + uySqr;

		// Weights
		field_t weightR	= (2 * rho[runIndex]) / 9;	// Rest
		field_t weightHV	= rho[runIndex] / 18;	// Horizontal/Vertical
		field_t weightD	= rho[runIndex] / 36;		// Diagonal

		// Calculate the rest equlibrium field component
		populationsEq[getArrayIndex(runIndex, CellDirection::rest)] = weightR * (2 - (3 * uSqr));

		// Calculate horizontal and vertical equlibrium field components
		populationsEq[getArrayIndex(runIndex, CellDirection::east)]	= weightHV * (2 + (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::north)]	= weightHV * (2 + (6 * uy) + (9 * uySqr) - (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::west)]	= weightHV * (2 - (6 * ux) + (9 * uxSqr) - (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::south)] = weightHV * (2 - (6 * uy) + (9 * uySqr) - (3 * uSqr));

		// Calculate diagonal equlibrium field components
		populationsEq[getArrayIndex(runIndex, CellDirection::northEast)] = weightD * (1 + (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::northWest)] = weightD * (1 - (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::southWest)] = weightD * (1 - (3 * (ux + uy)) + (9 * uxuy) + (3 * uSqr));
		populationsEq[getArrayIndex(runIndex, CellDirection::southEast)] = weightD * (1 + (3 * (ux - uy)) - (9 * uxuy) + (3 * uSqr));
	}


	//*****************************************************************************************
	// Set functions
	void setPopulation(const bool runIndex, const int populationIndex, const field_t fieldValue) {
		int arrayIndex = getArrayIndex(runIndex, populationIndex);
		//std::cout << "setPopulation ARRAY_INDEX : " << arrayIndex << std::endl;;
		assert(("setPopulations: arrayIndex is negative", arrayIndex >= 0));
		assert(("setPopulations: arrayIndex to high", arrayIndex < nFieldDuplicates * nPopulations));
		populations[arrayIndex] = fieldValue;
	}

	void setRho(const bool runIndex, const field_t rho_) {
		rho[runIndex] = rho_;
	}

	void setVelocity(const bool runIndex, const SpatialDirection direction, const field_t velocity_) {
		int arrayIndex = runIndex * nDimensions + direction;
		std::cout << "setVelocity ARRAY_INDEX : " << arrayIndex << std::endl;
		assert(("setVelocity: arrayIndex is negative", arrayIndex >= 0));
		assert(("setVelocity: arrayIndex to high", arrayIndex < nFieldDuplicates * nDimensions));
		velocity[arrayIndex] = velocity_;
	}
	

	//*****************************************************************************************
	// Get functions

	Neighbours &getCellNeighbours() {
		return neighbours;
	}

	const std::string getPopulationsList(const bool runIndex) {
		std::string temp;
		temp += "{" + std::to_string(populations[getArrayIndex(runIndex, 0)]);
		for (int i = 1; i < nPopulations-1; i++) {
			temp += ", " + std::to_string(populations[getArrayIndex(runIndex, i)]);
			
		}
		
		temp += ", " + std::to_string(populations[getArrayIndex(runIndex, nPopulations-1)]) + "}";
		return temp;
	}


	const field_t getPolulation(const bool runIndex, const int populationIndex) const {
		int arrayIndex = getArrayIndex(runIndex, populationIndex);
		//std::cout << "getPopulation ARRAY_INDEX : " << arrayIndex << std::endl;;
		assert(("getPopulations: arrayIndex is negative", arrayIndex >= 0));
		assert(("getPopulations: arrayIndex to high", arrayIndex < nFieldDuplicates * nPopulations));
		return populations[arrayIndex];
	}

	const field_t getRho(const bool runIndex) const {
		return rho[runIndex];
	}

	const field_t getVelocity(const bool runIndex, const SpatialDirection direction) const{
		return velocity[runIndex * nDimensions + direction];
	}

	const std::string getVelocityList(const bool runIndex) {
		std::string temp;
		temp += "{" + std::to_string(velocity[runIndex * nDimensions + SpatialDirection::x]);
		temp += ", " + std::to_string(velocity[runIndex * nDimensions + SpatialDirection::y]) + "}";
		return temp;
	}

	virtual char getCellTypeChar() const{
		return 'C';
	}


	virtual CellType getCellType() const{
		return CellType::cell;
	}


	const int getNumberOfPopulations() {
		return nPopulations;
	}

	const int getNumberOfFieldDuplicates() {
		return nFieldDuplicates;
	}	
};