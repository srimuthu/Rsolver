#pragma once
#include "RsolverTypes.h"

namespace Rsolver {

class IRsolverUtils
{
	public:
	virtual ~IRsolverUtils() = default;
	// Takes a Cube Vector as input and converts it
	// to the UFRDBL notation string
	virtual CubeStateInUFRDBL GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors) = 0;
};
}

