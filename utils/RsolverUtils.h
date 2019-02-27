#pragma once
#include "IRsolverUtils.h"

namespace Rsolver {

class RsolverUtils : public IRsolverUtils
{
	public:
		explicit RsolverUtils(CubeStateInUFRDBL solutionState, const std::string& cubeStatesTxtFile);
		~RsolverUtils() = default;

		virtual CubeStateInUFRDBL GetCubeStateInUFRDBL(const CubeStateInColors& cubeStateInColors) override;

	private:

		CubeStateInUFRDBL		m_solutionState;
		CubeState				m_defaultStateMap;
};
}

