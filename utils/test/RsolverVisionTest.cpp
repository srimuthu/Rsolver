#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "RsolverHelpers.h"
#include "RsolverVision.h"
#include "opencv2/highgui.hpp"

namespace Rsolver
{
namespace Test
{

using namespace testing;
const bool g_debugImages = true;
#ifdef _WIN32
static const std::string g_testDataFilesPath = "..\\..\\..\\rsolver\\data\\test_data\\";
#else
static const std::string g_testDataFilesPath = "../../../rsolver/data/test_data/";
#endif

class RsolverVisionTest : public testing::Test
{
public:
	void SetUp() override
	{
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

	std::unique_ptr<RsolverVision>	m_rsolverVision;
};

TEST_F(RsolverVisionTest, DISABLED_CaptureImageFromSensor)
{
	// Enable this test to check if camera works
	auto img = m_rsolverVision->CaptureImageFromSensor();
	EXPECT_EQ(g_defaultHeight, img.size().height);
	EXPECT_EQ(g_defaultWidth, img.size().width);
	if (g_debugImages)
	{
		cv::imshow("Sample", img);
		cv::waitKey(0);
	}
}

TEST_F(RsolverVisionTest, DISABLED_GetImagesForTesting)
{
	// Enable this test to capture new test data
	m_rsolverVision->CalibrateCubeCameraDistanceGUI(true);
}


TEST_F(RsolverVisionTest, DISABLED_WriteCubiesToDisk)
{
	// Enable this test to see if the cubies are being cropped properly
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		for (auto j = 0; j < g_cubiesPerFace; j++)
		{
			cv::Mat cubieImage = m_rsolverVision->GetCubieAtIndex(img, j);
			std::string outputFileName = "face_" + std::to_string(i) + "_cubie_" + std::to_string(j) + ".png";
			cv::imwrite(outputFileName, cubieImage);
		}
	}
}

TEST_F(RsolverVisionTest, CalibrateBoundariesForTestDataImages)
{
	// Read from the disk
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = g_testDataFilesPath + "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		m_rsolverVision->PerformBoundariesCalibrationByFaceColor(img, static_cast<Colors>(i));
	}

	// Read from the disk
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

	if (g_debugImages)
	{
		Helpers::PrintCubeStateInColorsToConsole(cubeStateInColorsActual);
	}
}

}
}
