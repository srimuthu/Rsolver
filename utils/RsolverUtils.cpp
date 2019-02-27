#include "RsolverUtils.h"
#include "RsolverHelpers.h"

namespace Rsolver {

RsolverUtils::RsolverUtils(CubeStateInUFRDBL solutionState, const std::string& cubeStatesTxtFile)
	: m_solutionState(solutionState)
	, m_defaultStateMap(Helpers::ParseCubeStatesTxtFile(cubeStatesTxtFile))
{
	auto defaultSolvedState = Helpers::GetDefaultSolvedCubeStateInColors();
}

CubeStateInUFRDBL RsolverUtils::GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors)
{
	//Call helper with cubeStateInColors, solutionState and defaultStateMap
	return Helpers::GetCubeStateInUFRDBL(cubeStateInColors, m_solutionState, m_defaultStateMap);
}

}

