#pragma once
#include "RsolverTypes.h"

namespace Rsolver {

class IRsolverUtils
{
	public:
	virtual ~IRsolverUtils() = default;

	// Takes a Cube Vector as input and converts it to the UFRDBL notation string
	virtual CubeStateInUFRDBL GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors) = 0;

	// Takes current state of cube in UFRDBL and returns solution in UFRDBL clockwise moves
	virtual std::string SolveCubeFromGivenState(const CubeStateInUFRDBL& cubeStateInUfrdbl) = 0;

};
}

