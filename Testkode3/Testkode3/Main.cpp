#pragma once
#include "Globals.hpp"
#include "Grid.hpp"



int main() {
	bool runIndex = 0;
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


#if 1
	int nRun = 5;
	grid.printCellPopulation(runIndex);
	system("pause");
	for (int run = 0; run < nRun; run++) {
		grid.collide(runIndex);
		grid.propagate(runIndex);
		grid.printCellPopulation(!runIndex);
		system("pause");
		runIndex = !runIndex;
	}
#endif
}