#include "Globals.hpp"
#include "Grid.hpp"
#include "Cell.hpp"
#include "BulkCell.hpp"
#include "SolidCell.hpp"
#include "Dynamics.hpp"
#include "BounceBack.hpp"
#include "Neighbours.hpp"
#include <array>
#include <iostream>
#include <memory>








void setPoulation(int runIndex, int x, int y) {	
	for (int cellDirection = 0; cellDirection < nPopulations; cellDirection++) {
		grid[gridPosition(x, y)]->setPopulation(runIndex, cellDirection, cellDirection);
	}	
}





void addSolidNeighbours() {
	std::shared_ptr<Cell> solidNeighbourCell;
	Neighbours neighbours;
	for (int y = 0; y < yDim; y++) {
		for (int x = 0; x < xDim; x++) {
			if (geometry[(y * xDim) + x] == 0) {
				for (int direction = 0; direction < nDirections; direction++) {					
					neighbours = grid[gridPosition(x, y)]->getCellNeighbours();
					solidNeighbourCell = neighbours.getNeighbour(direction);
					if (solidNeighbourCell == nullptr) {
						neighbours.initializeNeighbour<SolidCell>(direction);						
						grid[gridNeigbourPossition(x, y, direction)] = neighbours.getNeighbour(direction);
					}
				}
			}
		}
	}
}

void initializeSolidCellNeighbours(){
	std::shared_ptr<Cell> solidCellNeighbourCell;
	std::shared_ptr<Cell> solidCell;
	for (int y = 0; y < yDim; y++) {
		for (int x = 0; x < xDim; x++) {
			//if (geometry[(y * xDim) + x] == 1) {			
			solidCell = grid[gridPosition(x, y)];
			if (solidCell != nullptr){
				if (solidCell->getCellType() == CellType::solidCell) {
					for (int direction = 0; direction < nDirections; direction++) {
						solidCellNeighbourCell = grid[gridNeigbourPossition(x, y, direction)];
						if (solidCellNeighbourCell != nullptr) {
							solidCell->getCellNeighbours().setNeighbour(direction, solidCellNeighbourCell);
						}
						else {
							solidCell->getCellNeighbours().setNeighbour(direction, solidCell);
						}
					}
				}
			//	}
			}
		}
	}
}









//void printCellPopulation(int x, int y) {
//	std::cout << std::endl;
//	for (int cellDirection = 0; cellDirection < 9; cellDirection++) {		
//		std::cout << grid[gridPosition(x, y)]->getPolulation(cellDirection);
//	}
//}

void printNeighbourCellPopulation(bool runIndex, int x, int y) {
	std::cout << std::endl;
	std::cout << grid[gridPosition(x - 1, y - 1)]->getPolulation(runIndex, CellDirection::northWest)
		<< " " << grid[gridPosition(x + 0, y - 1)]->getPolulation(runIndex, CellDirection::north)
		<< " " << grid[gridPosition(x + 1, y - 1)]->getPolulation(runIndex, CellDirection::northEast) << std::endl;
	
	std::cout << " \\|/ " << std::endl;

	std::cout << grid[gridPosition(x - 1, y + 0)]->getPolulation(runIndex, CellDirection::west)
		<< "-" << grid[gridPosition(x + 0, y + 0)]->getPolulation(runIndex, CellDirection::rest)
		<< "-" << grid[gridPosition(x + 1, y + 0)]->getPolulation(runIndex, CellDirection::east) << std::endl;

	std::cout << " /|\\ " << std::endl;
	
	std::cout << grid[gridPosition(x - 1, y + 1)]->getPolulation(runIndex, CellDirection::southWest)
		<< " " << grid[gridPosition(x + 0, y + 1)]->getPolulation(runIndex, CellDirection::south)
		<< " " << grid[gridPosition(x + 1, y + 1)]->getPolulation(runIndex, CellDirection::southEast) << std::endl;
}





void printRow(bool runIndex, int y) {
	for (int x = xDim - 1; x > 1 ; x--) {
		std::cout << grid[gridPosition(x, y)]->getPolulation(runIndex, 4);
	}
}



class TestClass {

protected:
	std::array<std::shared_ptr<Cell>, 3> cells;

public:

	TestClass() = default;
	~TestClass() = default;

	std::shared_ptr<Cell> getCell(int index) {
		return cells[index];
	}
	void setCell(int index, std::shared_ptr<Cell> cell_) {
		cells[index] = cell_;
	}
	template<typename CellType>
	void initializeCell(int index) {
		cells[index] = std::make_shared<CellType>();
	}

};
//int main() {
#if 0
	for (int i = 0; i < 8; i++) {
		std::cout << i << " - " << reverseDirectionIndex(i) << std::endl;
	}
	system("pause");
#endif


#if 0
	TestClass testClass;

	std::shared_ptr<Cell> cell1 = testClass.getCell(1);

	testClass.initializeCell<SolidCell>(1);

	std::shared_ptr<Cell> cell2 = testClass.getCell(1);
	
	
	std::cout << cell1->getCellTypeChar();
	//cell = std::dynamic_pointer_cast<SolidCell, Cell>(cell);

	std::cout << cell2->getCellTypeChar();

	std::cin.get();
#endif



#if 1
	makeGeometry();
	printGeometry();

	//grid[(1 * xDim) + 1] = std::make_shared<BulkCell>();
	//grid[gridPosition(1, 1)]->setPopulation(0, 0, 0);

	makeGrid();
	initializeNeighbours();
	addSolidNeighbours();
	initializeSolidCellNeighbours();
	initializeNeighbours();
	printCellRho();
	printCellType();



	
	grid[gridPosition(1,1)]->getCellNeighbours().getNeighbour(1)->setRho(99);


	const int X = 2;
	const int Y = 2;
	bool runIndex = 0;

	printNeighboursCellType(X, Y);

	//printCellRho();
	setPoulation(1, X, Y);
	
	//printCellPopulation(0, X, Y);
	
	//grid[gridPosition(X, Y)]->propageteTo(runIndex);
	//
	//printCellPopulation(!runIndex, X+1, Y-1);
	
	for (int i = 0; i < 9; i++) {
		
		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (grid[gridPosition(x, y)] != nullptr) {
					//for (int y = 0; y < yDim; y++) {
						//for (int x = 0; x < xDim; x++) {
					if (grid[gridPosition(x, y)] != nullptr) {
						printCellPopulation(0, x, y);
						printCellPopulation(1, x, y);
						//}
					//}
					}
				}
			}
		}


		for (int y = 0; y < yDim; y++) {
			for (int x = 0; x < xDim; x++) {
				if (grid[gridPosition(x, y)] != nullptr) {
					grid[gridPosition(x, y)]->propageteTo(runIndex);
					//grid[gridPosition(x, y)]->collide(runIndex);
				}
			}
		}

		

		runIndex = !runIndex;
		//printCellPopulation(0, X, Y);
		//printCellPopulation(1, X, Y);
	/*	printCellPopulation(0, X-1, Y+1);
		printCellPopulation(1, X-1, Y+1);*/
		//printRow(0, 2);

		

		


		system("pause");
 	}
	//grid[gridPosition(X, Y)]->propageteTo(runIndex);
	

	system("pause");
	

#endif

	
	

// }

