#pragma once
#include "Globals.hpp"
#include "Grid.hpp"
#include <iostream>
#include<fstream>
#include <string>
#include <exception>

// !!!!!*************************!!!!!************************************!!!!!
// The use of hpp files for almost all code in this project is done in order to
// make code development faster. Most code will be moved to cpp files later on.
// Templates will remain in the header files.


void stringToFile(const std::string string, const std::string filename){
	std::ofstream myfile(filename);
	std::string outputString = string;
	outputString.insert(0, "{");
	outputString.append("}");

	if (myfile.is_open())
	{
		myfile << outputString;
		myfile.close();
	}
	else std::cout << "Unable to open file";
}

//void addGridPopulationsToString(const bool runIndex, const Grid& grid, std::string& populationsList) {
//	populationsList += "\n,{" + grid.getGridPolulationsList(runIndex) + "}";
//}




int main() {

	bool runIndex = 0;
	field_t rho = 1;
	field_t velocity = 0;
	Grid grid;

	std::string outputString = "";

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
	//grid.gridInitialize(!runIndex);
	
#if 0
	std::cout << "Initial population for runIndex" << std::endl;
	grid.printCellPopulation(runIndex);
	std::cout << "Initial population for !runIndex" << std::endl;
	grid.printCellPopulation(!runIndex);
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
	int nRun = 10;
	/*grid.printCellRho(runIndex);
	grid.printCellRho(!runIndex);
	system("pause");*/
	/*std::cout << "Initial population for runIndex" << std::endl;
	grid.printCellPopulation(runIndex);
	std::cout << "Initial population for !runIndex" << std::endl;
	grid.printCellPopulation(!runIndex);*/
	/*system("pause");*/
	
	/*grid.getCell(3, 2)->setVelocity(runIndex, SpatialDirection::x, 0.1);
	grid.getCell(3, 2)->setVelocity(runIndex, SpatialDirection::y, 0.1);*/
	//grid.getCell(2, 2)->setRho(runIndex, 3);
	//grid.getCell(2, 2)->setRho(runIndex, 3);
	//grid.getCell(2, 2)->setRho(!runIndex, 3);
	grid.getCell(2, 2)->initializeRho(runIndex, 3);
	//grid.getCell(2, 2)->getRho(runIndex)
	outputString = "";
	//grid.appendGridPolulationsList(!runIndex, outputString);
	for (int run = 0; run < nRun; run++) {
		std::cout << "RUN: " << run << std::endl;
		//grid.printCellType();		
		std::cout << "Rho before collide:" << grid.getCell(2, 2)->getRho(runIndex) << "," << grid.getCell(2, 2)->getRho(!runIndex) << "\n";
		grid.collide(runIndex);
		std::cout << "Rho after collide:" << grid.getCell(2, 2)->getRho(runIndex) << "," << grid.getCell(2, 2)->getRho(!runIndex) << "\n";
		grid.appendGridPolulationsList(runIndex, outputString);		
		grid.propagate(runIndex);
		std::cout << "Rho before propagate:" << grid.getCell(2, 2)->getRho(runIndex) << "," << grid.getCell(2, 2)->getRho(!runIndex) << "\n";
		grid.appendGridPolulationsList(!runIndex, outputString);
		//grid.collideAndPropagate(runIndex);
		//grid.printCellPopulation(!runIndex);
		grid.printCellRho(runIndex);
		grid.printCellRho(!runIndex);
		stringToFile(outputString, "testfile.txt");
		
		system("pause");
		
		runIndex = !runIndex;
		
	}
#endif

#if 0
	BulkCell cell;
	cell.getPolulation(!runIndex, 9);
	system("pause");

#endif



#if 0
	std::array<int, 2> arrayy = { 1,2 };
	try {
		arrayy[2] = 0;		
	}
	catch(std::exception& e){
		std::cout << "Catch" << e.what();
		system("pause");
	}
	std::cout << arrayy[2];
	system("pause");
#endif

	
	
}