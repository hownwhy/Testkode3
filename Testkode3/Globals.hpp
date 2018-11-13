#pragma once
#include <assert.h>

typedef double field_t;






enum  CellDirection : int
{
	east = 0,
	northEast = 1,
	north = 2,
	northWest = 3,
	west = 4,
	southWest = 5,
	south = 6,
	southEast = 7,
	rest = 8
};


const static int nDirections = 8;
const static int threeHalfDirection = (3 * nDirections) / 2;
static int reverseDirectionIndex(const int directionIndex) {
	return (directionIndex + threeHalfDirection) % nDirections;
}
