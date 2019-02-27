#include "RsolverUtils.h"
#include "RsolverHelpers.h"
#include "Solver.h"

namespace Rsolver {

RsolverUtils::RsolverUtils(CubeStateInUFRDBL solutionState, const std::string& cubeStatesTxtFile)
	: m_solutionState(solutionState)
	, m_defaultStateMap(Helpers::ParseCubeStatesTxtFile(cubeStatesTxtFile))
{
	auto defaultSolvedState = Helpers::GetDefaultSolvedCubeStateInColors();
}

CubeStateInUFRDBL RsolverUtils::GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors)
{
	CubeStateInUFRDBL cubeStateInUfrdbl;
	auto solutionStateTokens = Helpers::SplitStringBySpace(m_solutionState);
	for (auto const& token : solutionStateTokens)
	{
		if (Helpers::IsTokenAnEdge(token))
		{
			auto edgeCube = m_defaultStateMap.edgeMap.find(token)->second;
			auto actualOrientationId = Helpers::GetEdgeOrientationFromCubeStateInColors(cubeStateInColors, edgeCube);
			auto actualKey = Helpers::GetKeyForOrientation(m_defaultStateMap, actualOrientationId, CubeType::Edge);
			cubeStateInUfrdbl = cubeStateInUfrdbl + actualKey + " ";
		}
		else
		{
			{
				auto cornerCube = m_defaultStateMap.cornerMap.find(token)->second;
				auto actualOrientationId = Helpers::GetCornerOrientationFromCubeStateInColors(cubeStateInColors, cornerCube);
				auto actualKey = Helpers::GetKeyForOrientation(m_defaultStateMap, actualOrientationId, CubeType::Corner);
				cubeStateInUfrdbl = cubeStateInUfrdbl + actualKey + " ";
			}
		}
	}
	return Helpers::trim(cubeStateInUfrdbl);
}

std::string RsolverUtils::SolveCubeFromGivenState(const CubeStateInUFRDBL & cubeStateInUfrdbl)
{
	std::vector<std::string> scrambledState = Helpers::SplitStringBySpace(cubeStateInUfrdbl);
	std::vector<std::string> solutionState = Helpers::SplitStringBySpace(m_solutionState);
	return Thistlethwaite::SolveCubeFromGivenState(scrambledState, solutionState);
}

}

