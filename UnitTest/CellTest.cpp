#include "stdafx.h"
#include "CppUnitTest.h"
#include "\Programming\Repositories\Testkode3\Testkode3\Cell.hpp"
#include "\Programming\Repositories\Testkode3\Testkode3\BulkCell.hpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(CellTest)
	{
	private:
		Cell cell;
		const bool RUNINDEX_0 = 0;
		const int nPopulationsX2 = cell.getNumberOfPopulations() * cell.getNumberOfFieldDuplicates();
		int runIndex = 0;

		Neighbours neighbours;
		std::array<field_t, nFieldDuplicates * nPopulations> expectedPopulations;
		std::array<field_t, nFieldDuplicates * nPopulations> expectedPopulationsEq;
		std::array<field_t, nFieldDuplicates> expectedRho;
		std::array<field_t, nFieldDuplicates * nDimensions> expectedVelocity;

	public:
		
		TEST_METHOD(getArrayIndex)
		{			
			std::array<int, 4> expected = { 0, 8, 9, 17 };
			std::array<int, 4> actual;			

			actual[0] = Cell::getArrayIndex(0, 0);
			actual[1] = Cell::getArrayIndex(0, 8);
			actual[2] = Cell::getArrayIndex(1, 0);
			actual[3] = Cell::getArrayIndex(1, 8);

			Assert::IsTrue(actual == expected);			
		}

		TEST_METHOD(computeRho)
		{
			
			field_t actualRho;
			field_t expectedRho = 0;
			
			for (int iPopulation = 0; iPopulation < nPopulationsX2; iPopulation++) {
				cell.setPopulation(RUNINDEX_0, iPopulation, iPopulation + 0.13);
				expectedRho += iPopulation + 0.13;
			}
			cell.computeRho(RUNINDEX_0);
			cell.computeRho(!RUNINDEX_0);
			actualRho = cell.getRho(RUNINDEX_0) + cell.getRho(!RUNINDEX_0);
			Assert::AreEqual(actualRho, expectedRho);
		}

		TEST_METHOD(computeVelocity) {
			//Cell cell;
			field_t actualVelocityX;
			field_t actualVelocityY;
			field_t expectedVelocityX = 0;
			field_t expectedVelocityY = 0;
			for (int iPopulation = 0; iPopulation < nPopulationsX2; iPopulation++) {
				cell.setPopulation(RUNINDEX_0, iPopulation, iPopulation + 0.13);
				expectedRho += iPopulation + 0.13;
			}


		}
	};


}