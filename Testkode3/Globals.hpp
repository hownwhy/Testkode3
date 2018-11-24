#pragma once
#include <assert.h>

typedef double field_t;

// The "rest" direction has been given the index 8, as opposed to the more usual 0.
// This was done for compability with the directions indexes starting at east = 0.
// This might not be necessary.
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

// This function gives the opposite direction of what you put in.
// This is also the reason why I chose to not follow the convension
// having the rest direction be the 0 direction. 
inline int reverseDirectionIndex(const int directionIndex) {
	
	return (directionIndex + threeHalfDirection) % nDirections;
}

