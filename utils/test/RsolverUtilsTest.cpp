#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

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
	static const std::string g_cubeStatesTxtFile = "..\\..\\..\\rsolver\\data\\CubeStates.txt";
#else
	static const std::string g_cubeStatesTxtFile = "../../../rsolver/data/CubeStates.txt";
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

		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[0] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[1] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[2] = Colors::Red;

		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[0] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[1] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[2] = Colors::Blue;

		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[0] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[1] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[2] = Colors::Orange;

		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[0] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[1] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[2] = Colors::Green;

		return cubeStateInColors;
	}

	CubeStateInColors CreateNonDefaultCubeStateSeq1()
	{
		CubeStateInColors cubeStateInColors = Helpers::GetDefaultSolvedCubeStateInColors();
		// White face (Up)
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[0] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[1] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[2] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[3] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[4] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[5] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[6] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[7] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Up)].faceColorVector[8] = Colors::Orange;
		// Green face (Front)
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[0] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[1] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[2] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[3] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[4] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[5] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[6] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[7] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Front)].faceColorVector[8] = Colors::Red;
		// Red face (Right)
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[0] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[1] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[2] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[3] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[4] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[5] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[6] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[7] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Right)].faceColorVector[8] = Colors::White;
		// Yellow face (Down)
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[0] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[1] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[2] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[3] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[4] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[5] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[6] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[7] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Down)].faceColorVector[8] = Colors::Red;
		// Blue face (Back)
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[0] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[1] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[2] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[3] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[4] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[5] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[6] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[7] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Back)].faceColorVector[8] = Colors::Orange;
		// Organge face (Left)
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[0] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[1] = Colors::Red;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[2] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[3] = Colors::Yellow;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[4] = Colors::Orange;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[5] = Colors::Green;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[6] = Colors::Blue;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[7] = Colors::White;
		cubeStateInColors[static_cast<int>(CubeFaces::Left)].faceColorVector[8] = Colors::Blue;

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

TEST_F(RsolverUtilsTest, VerifyNonDefaultCubeStateComplexShuffle)
{
	// Returns cube state with F- U+ F- D- L- D- F- U- L2 D- performed on solved cube
	CubeStateInColors cubeStateInColors = CreateNonDefaultCubeStateSeq1();
	CubeStateInUFRDBL expectedState = "RU LF UB DR DL BL UL FU BD RF BR FD LDF LBD FUL RFD UFR RDB UBL RBU";
	EXPECT_EQ(expectedState, m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColors));
}

TEST_F(RsolverUtilsTest, GetSolutionMovesForState1U)
{
	// Returns cube state with one U+ performed on solved cube
	CubeStateInColors cubeStateInColors = CreateNonDefaultCubeState1U();
	CubeStateInUFRDBL currentState = "UR UB UL UF DF DR DB DL FR FL BR BL URB UBL ULF UFR DRF DFL DLB DBR";
	EXPECT_EQ(currentState, m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColors));
	std::string expectedSolution = "U1U2";
	EXPECT_EQ(expectedSolution, m_rsolverUtils->SolveCubeFromGivenState(currentState));
}

TEST_F(RsolverUtilsTest, GetSolutionMovesForStateComplexShuffle)
{
	// Returns cube state with one U+ performed on solved cube
	CubeStateInColors cubeStateInColors = CreateNonDefaultCubeStateSeq1();
	CubeStateInUFRDBL currentState = "RU LF UB DR DL BL UL FU BD RF BR FD LDF LBD FUL RFD UFR RDB UBL RBU";
	EXPECT_EQ(currentState, m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColors));
	std::string expectedSolution = "U1U2";
	EXPECT_EQ(expectedSolution, m_rsolverUtils->SolveCubeFromGivenState(currentState));
}

}
}
