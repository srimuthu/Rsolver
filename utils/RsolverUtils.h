#pragma once
#include <memory>
#include "IRsolverUtils.h"
#include "ThistlethwaiteSolver.h"

namespace Rsolver {

class RsolverUtils : public IRsolverUtils
{
	public:
		explicit RsolverUtils(CubeStateInUFRDBL solutionState, const std::string& cubeStatesTxtFile);
		~RsolverUtils() = default;

		virtual CubeStateInUFRDBL GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors) override;
		virtual std::string SolveCubeFromGivenState(const CubeStateInUFRDBL& cubeStateInUfrdbl) override;

	private:

		CubeStateInUFRDBL										m_solutionState;
		CubeState												m_defaultStateMap;
		std::unique_ptr<Thistlethwaite::ThistlethwaiteSolver>	m_thistlethwaiteSolver;
};
}

