#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "RsolverVision.h"
#include "RsolverUtils.h"
#include "RsolverHelpers.h"
#include "RsolverSerial.h"
#include "RsolverControl.h"

namespace Rsolver
{
namespace Test
{

using namespace testing;
static const int g_baud = 9600;
const bool g_debugImages = true;
static const Rsolver::CubeStateInUFRDBL g_solutionState = "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR";
#ifdef _WIN32
static const int g_deviceId = 1;	// 1 = Use Droid Cam on windows
static const std::string g_testDataFilesPath = "..\\..\\..\\rsolver\\data\\test_data\\";
static const std::string g_cubeStatesTxtFile = "..\\..\\..\\rsolver\\data\\CubeStates.txt";
static const std::string g_portName = "COM16";
#else
static const int g_deviceId = 0;
static const std::string g_testDataFilesPath = "../../../rsolver/data/test_data/";
static const std::string g_cubeStatesTxtFile = "../../../rsolver/data/CubeStates.txt";
static const std::string g_portName = "ToBeImplemented";
#endif

class RsolverIntegrationTest : public testing::Test
{
public:
	void SetUp() override
	{
		int baud = g_baud;
		m_rsolverSerial = std::make_unique<RsolverSerial>(g_portName, baud);
		m_rsolverUtils = std::make_unique<RsolverUtils>(g_solutionState, g_cubeStatesTxtFile);
		m_rsolverVision = std::make_unique<RsolverVision>(g_deviceId);
		m_rsolverControl = std::make_unique<RsolverControl>(std::move(m_rsolverSerial));
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

	std::unique_ptr<IRsolverVision>		m_rsolverVision;
	std::unique_ptr<IRsolverUtils>		m_rsolverUtils;
	std::unique_ptr<IRsolverSerial>		m_rsolverSerial;
	std::unique_ptr<IRsolverControl>	m_rsolverControl;
};

TEST_F(RsolverIntegrationTest, DISABLED_SerialPortTest)
{
	m_rsolverControl->InitializeRobot();
	auto commands = m_rsolverControl->GenerateLockCubeInPlaceCommands();
	m_rsolverControl->ExecuteRobotCommands(commands);
}

TEST_F(RsolverIntegrationTest, DISABLED_CaptureImageFromSensor)
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

TEST_F(RsolverIntegrationTest, DISABLED_CalibrateCubeCameraDistanceTest)
{
	m_rsolverVision->CalibrateCubeCameraDistanceGUI(false);
}

TEST_F(RsolverIntegrationTest, DISABLED_GetImagesForTesting)
{
	// Enable this test to capture new test data
	m_rsolverVision->CalibrateCubeCameraDistanceGUI(true);
}

TEST_F(RsolverIntegrationTest, DISABLED_GetSolutionForTestDataImages)
{
	// Perform Calibration from Test Images
	for (int i = 0; i < g_numFaces; i++)
	{
		std::string inputFileName = g_testDataFilesPath + "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		m_rsolverVision->PerformBoundariesCalibrationByFaceColor(img, static_cast<Colors>(i));
	}

	// Identify colors of cubies in Test Images
	CubeStateInColors cubeStateInColorsActual;
	for (int i = 0; i < g_numFaces; i++)
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

TEST_F(RsolverIntegrationTest, FullWorkFlow)
{
	m_rsolverControl->InitializeRobot();
	std::cout << "Place cube" << std::endl;
	auto lockCubeCommands = m_rsolverControl->GenerateLockCubeInPlaceCommands();
	std::string stage = "Locking Cube in Place";
	m_rsolverControl->SetProgressUpdateCallback([&stage](Progress progress) {
		Helpers::PrintProgressUpdate(progress, stage);
	});
	m_rsolverControl->ExecuteRobotCommands(lockCubeCommands);

	m_rsolverVision->CalibrateCubeCameraDistanceGUI(false);

	std::vector<cv::Mat> cubeFaceImages;
	for (int i = 0; i < g_numFaces; i++)
	{
		stage = "Preparing for capture face " + std::to_string(i);
		auto facePhoto = m_rsolverControl->GeneratePrepareForCapture(static_cast<CubeFaces>(i));
		m_rsolverControl->ExecuteRobotCommands(facePhoto);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		auto img = m_rsolverVision->CaptureImageFromSensor();
		cubeFaceImages.emplace_back(img);
		m_rsolverVision->PerformBoundariesCalibrationByFaceColor(img, static_cast<Colors>(i));
		stage = "Recovering from capture face " + std::to_string(i);
		auto faceRecover = m_rsolverControl->GenerateRecoverFromCapture(static_cast<CubeFaces>(i));
		m_rsolverControl->ExecuteRobotCommands(faceRecover);
	}

	CubeStateInColors cubeStateInColorsActual;
	for (int i = 0; i < g_numFaces; i++)
	{
		auto cubeFaceInfo = m_rsolverVision->GetCubeFaceInfoColorsFromImage(cubeFaceImages.at(i));
		cubeStateInColorsActual.emplace_back(cubeFaceInfo);
	}

	std::cout << "\nDetected Cube State :" << std::endl;
	Helpers::PrintCubeStateInColorsToConsole(cubeStateInColorsActual);

	std::cout << "Solving . . ." << std::endl;
	auto cubeStateInUfrdbl = m_rsolverUtils->GetCubeStateInUFRDBL(cubeStateInColorsActual);
	auto cubeSolution = m_rsolverUtils->SolveCubeFromGivenState(cubeStateInUfrdbl);

	stage = "Solving Cube ";
	auto commands = m_rsolverControl->GetRobotCommandsFromSolution(cubeSolution);
	m_rsolverControl->ExecuteRobotCommands(commands);
}

}
}
