#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "RsolverUtils.h"
#include "RsolverHelpers.h"

namespace Rsolver
{
namespace Test
{

using namespace testing;

namespace 
{
	static const Rsolver::CubeStateInUFRDBL g_solutionState = "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR";
#ifdef _WIN32
	static const std::string g_cubeStatesTxtFile = "D:\\sb\\rsolver\\data\\CubeStates.txt";
#else
	static const std::string g_cubeStatesTxtFile = "D:/sb/rsolver/data/CubeStates.txt";
#endif
}

class RsolverUtilsTest : public testing::Test
{
public:
	void SetUp() override
	{
		m_rsolverUtils = std::make_unique<RsolverUtils>(g_solutionState, g_cubeStatesTxtFile);
	}

	CubeStateInColors CreateNonDefaultCubeState1U()
	{
		CubeStateInColors cubeStateInColors = Helpers::GetDefaultSolvedCubeStateInColors();

		cubeStateInColors[static_cast<int>(Colors::Green)].faceColorVector[0] = Colors::Red;
		cubeStateInColors[static_cast<int>(Colors::Green)].faceColorVector[1] = Colors::Red;
		cubeStateInColors[static_cast<int>(Colors::Green)].faceColorVector[2] = Colors::Red;

		cubeStateInColors[static_cast<int>(Colors::Red)].faceColorVector[0] = Colors::Blue;
		cubeStateInColors[static_cast<int>(Colors::Red)].faceColorVector[1] = Colors::Blue;
		cubeStateInColors[static_cast<int>(Colors::Red)].faceColorVector[2] = Colors::Blue;

		cubeStateInColors[static_cast<int>(Colors::Blue)].faceColorVector[0] = Colors::Orange;
		cubeStateInColors[static_cast<int>(Colors::Blue)].faceColorVector[1] = Colors::Orange;
		cubeStateInColors[static_cast<int>(Colors::Blue)].faceColorVector[2] = Colors::Orange;

		cubeStateInColors[static_cast<int>(Colors::Orange)].faceColorVector[0] = Colors::Green;
		cubeStateInColors[static_cast<int>(Colors::Orange)].faceColorVector[1] = Colors::Green;
		cubeStateInColors[static_cast<int>(Colors::Orange)].faceColorVector[2] = Colors::Green;

		return cubeStateInColors;
	}

	std::unique_ptr<IRsolverUtils>	m_rsolverUtils;
};

TEST_F(RsolverUtilsTest, GetDefaultSolvedCubeState)
{
	CubeStateInColors cubeStateInColors = Helpers::GetDefaultSolvedCubeStateInColors();
	EXPECT_EQ(g_solutionState, m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColors));
}

TEST_F(RsolverUtilsTest, VerifyNonDefaultCubeState1U)
{
	// Returns cube state with one U+ performed on solved cube
	CubeStateInColors cubeStateInColors = CreateNonDefaultCubeState1U();
	CubeStateInUFRDBL expectedState = "UR UB UL UF DF DR DB DL FR FL BR BL URB UBL ULF UFR DRF DFL DLB DBR";
	EXPECT_EQ(expectedState, m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColors));
}
}
}
