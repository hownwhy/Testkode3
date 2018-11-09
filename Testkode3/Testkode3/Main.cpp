#pragma once
#include "Globals.hpp"
#include "Grid.hpp"



int main() {
	bool runIndex = 0;
	field_t rho = 1;
	field_t velocity = 0;
	Grid grid;

	grid.makeGeometry();
#if 0
	grid.printGeometry();
	system("pause");
#endif

	grid.makeGrid();
#if 1
	grid.printCellType();
	system("pause");
#endif	

	grid.linkNeighbours();
#if 0
	grid.printNeighboursCellType();
	system("pause");
#endif

#if 0
	grid.printCellPopulation(runIndex);
	system("pause");
#endif

#if 0
	grid.propagate(runIndex);
	grid.printCellPopulation(!runIndex);
	system("pause");
#endif

	grid.gridInitialize(runIndex);
#if 0	
	grid.printCellPopulation(runIndex);
	system("pause");
#endif

#if 0
	grid.printCellVelocity(runIndex);
	system("pause");
#endif



#if 0
	int nRun = 5;
	//grid.printCellPopulation(runIndex);
	//grid.printCellRho(runIndex);
	//grid.printCellRho(!runIndex);
	//system("pause");
	for (int run = 0; run < nRun; run++) {
		grid.collide(runIndex);
		//grid.printCellPopulation(runIndex);
		grid.printCellRho(runIndex);
		grid.printCellRho(!runIndex);
		//system("pause");
		
		grid.propagate(!runIndex);
		//grid.printCellPopulation(!runIndex);
		grid.printCellRho(runIndex);
		grid.printCellRho(!runIndex);
		system("pause");
		runIndex = !runIndex;
	}
#endif

#if 1
	int nRun = 5;
	for (int run = 0; run < nRun; run++) {
		grid.collideAndProppagate(runIndex);
		//grid.printCellPopulation(runIndex);
		grid.printCellRho(runIndex);
		grid.printCellRho(!runIndex);
		//system("pause");

		grid.propagate(!runIndex);
		//grid.printCellPopulation(!runIndex);
		grid.printCellRho(runIndex);
		grid.printCellRho(!runIndex);
		system("pause");
		runIndex = !runIndex;
	}
#endif
}