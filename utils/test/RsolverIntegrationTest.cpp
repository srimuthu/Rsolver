#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "RsolverVision.h"
#include "RsolverUtils.h"
#include "RsolverHelpers.h"

namespace Rsolver
{
namespace Test
{

using namespace testing;
const bool g_debugImages = true;
static const Rsolver::CubeStateInUFRDBL g_solutionState = "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR";
#ifdef _WIN32
static const std::string g_testDataFilesPath = "..\\..\\..\\rsolver\\data\\test_data\\";
static const std::string g_cubeStatesTxtFile = "..\\..\\..\\rsolver\\data\\CubeStates.txt";
#else
static const std::string g_testDataFilesPath = "../../../rsolver/data/test_data/";
static const std::string g_cubeStatesTxtFile = "../../../rsolver/data/CubeStates.txt";
#endif

class RsolverIntegrationTest : public testing::Test
{
public:
	void SetUp() override
	{
		m_rsolverUtils = std::make_unique<RsolverUtils>(g_solutionState, g_cubeStatesTxtFile);
		m_rsolverVision = std::make_unique<RsolverVision>();
	}

	CubeStateInColors GetExpectedCubeStateInColorsForTestData()
	{
		CubeStateInColors stateInColors;
		CubeFaceInfo faceUp;
		faceUp.cubeFace = CubeFaces::Up;
		faceUp.faceColorVector.emplace_back(Colors::Blue);
		faceUp.faceColorVector.emplace_back(Colors::Green);
		faceUp.faceColorVector.emplace_back(Colors::Orange);
		faceUp.faceColorVector.emplace_back(Colors::Blue);
		faceUp.faceColorVector.emplace_back(Colors::White);
		faceUp.faceColorVector.emplace_back(Colors::Green);
		faceUp.faceColorVector.emplace_back(Colors::White);
		faceUp.faceColorVector.emplace_back(Colors::Orange);
		faceUp.faceColorVector.emplace_back(Colors::Green);
		stateInColors.emplace_back(faceUp);
		CubeFaceInfo faceFront;
		faceFront.cubeFace = CubeFaces::Front;
		faceFront.faceColorVector.emplace_back(Colors::Orange);
		faceFront.faceColorVector.emplace_back(Colors::Yellow);
		faceFront.faceColorVector.emplace_back(Colors::Orange);
		faceFront.faceColorVector.emplace_back(Colors::Yellow);
		faceFront.faceColorVector.emplace_back(Colors::Green);
		faceFront.faceColorVector.emplace_back(Colors::Red);
		faceFront.faceColorVector.emplace_back(Colors::Red);
		faceFront.faceColorVector.emplace_back(Colors::Red);
		faceFront.faceColorVector.emplace_back(Colors::Green);
		stateInColors.emplace_back(faceFront);
		CubeFaceInfo faceRight;
		faceRight.cubeFace = CubeFaces::Right;
		faceRight.faceColorVector.emplace_back(Colors::Yellow);
		faceRight.faceColorVector.emplace_back(Colors::Orange);
		faceRight.faceColorVector.emplace_back(Colors::Blue);
		faceRight.faceColorVector.emplace_back(Colors::Blue);
		faceRight.faceColorVector.emplace_back(Colors::Red);
		faceRight.faceColorVector.emplace_back(Colors::Orange);
		faceRight.faceColorVector.emplace_back(Colors::Red);
		faceRight.faceColorVector.emplace_back(Colors::White);
		faceRight.faceColorVector.emplace_back(Colors::Orange);
		stateInColors.emplace_back(faceRight);
		CubeFaceInfo faceDown;
		faceDown.cubeFace = CubeFaces::Down;
		faceDown.faceColorVector.emplace_back(Colors::Green);
		faceDown.faceColorVector.emplace_back(Colors::White);
		faceDown.faceColorVector.emplace_back(Colors::Yellow);
		faceDown.faceColorVector.emplace_back(Colors::Blue);
		faceDown.faceColorVector.emplace_back(Colors::Yellow);
		faceDown.faceColorVector.emplace_back(Colors::Green);
		faceDown.faceColorVector.emplace_back(Colors::White);
		faceDown.faceColorVector.emplace_back(Colors::White);
		faceDown.faceColorVector.emplace_back(Colors::Green);
		stateInColors.emplace_back(faceDown);
		CubeFaceInfo faceBack;
		faceBack.cubeFace = CubeFaces::Back;
		faceBack.faceColorVector.emplace_back(Colors::Yellow);
		faceBack.faceColorVector.emplace_back(Colors::Red);
		faceBack.faceColorVector.emplace_back(Colors::Red);
		faceBack.faceColorVector.emplace_back(Colors::White);
		faceBack.faceColorVector.emplace_back(Colors::Blue);
		faceBack.faceColorVector.emplace_back(Colors::Yellow);
		faceBack.faceColorVector.emplace_back(Colors::White);
		faceBack.faceColorVector.emplace_back(Colors::Blue);
		faceBack.faceColorVector.emplace_back(Colors::Blue);
		stateInColors.emplace_back(faceBack);
		CubeFaceInfo faceLeft;
		faceLeft.cubeFace = CubeFaces::Left;
		faceLeft.faceColorVector.emplace_back(Colors::Yellow);
		faceLeft.faceColorVector.emplace_back(Colors::Yellow);
		faceLeft.faceColorVector.emplace_back(Colors::Blue);
		faceLeft.faceColorVector.emplace_back(Colors::Green);
		faceLeft.faceColorVector.emplace_back(Colors::Orange);
		faceLeft.faceColorVector.emplace_back(Colors::Red);
		faceLeft.faceColorVector.emplace_back(Colors::Red);
		faceLeft.faceColorVector.emplace_back(Colors::Orange);
		faceLeft.faceColorVector.emplace_back(Colors::White);
		stateInColors.emplace_back(faceLeft);

		return stateInColors;
	}

	std::unique_ptr<IRsolverVision>	m_rsolverVision;
	std::unique_ptr<IRsolverUtils>	m_rsolverUtils;
};

TEST_F(RsolverIntegrationTest, GetSolutionForTestDataImages)
{
	// Perform Calibration from Test Images
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = g_testDataFilesPath + "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		m_rsolverVision->PerformBoundariesCalibrationByFaceColor(img, static_cast<Colors>(i));
	}

	// Identify colors of cubies in Test Images
	CubeStateInColors cubeStateInColorsActual;
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = g_testDataFilesPath + "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		auto cubeFaceInfo = m_rsolverVision->GetCubeFaceInfoColorsFromImage(img);
		cubeStateInColorsActual.emplace_back(cubeFaceInfo);
	}

	CubeStateInColors cubeStateInColorsExpected = GetExpectedCubeStateInColorsForTestData();

	EXPECT_EQ(cubeStateInColorsActual, cubeStateInColorsExpected);

	std::string expectedSolution = "D3 L1 F3 D3 L3 F2 R1 U1 B2 D1 F2 R3 R2 U1 R2 U3 F2 R2 U3 R2 D1 F2 U3 L2 B2 L2 F2 U2 R2 D2 F2 D2 L2";
	// The algorithm implementation is deterministic, so it is safe to always expect same solution for a given input
	auto cubeStateInUfrdbl = m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColorsActual);
	EXPECT_EQ(expectedSolution, m_rsolverUtils->SolveCubeFromGivenState(cubeStateInUfrdbl));
}

}
}
