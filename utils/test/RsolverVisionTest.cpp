#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "RsolverVision.h"
#include "opencv2/highgui.hpp"

namespace Rsolver
{
namespace Test
{

using namespace testing;
const bool g_debugImages = true;

class RsolverVisionTest : public testing::Test
{
public:
	void SetUp() override
	{
		m_rsolverVision = std::make_unique<RsolverVision>();
	}

	std::unique_ptr<RsolverVision>	m_rsolverVision;
};

TEST_F(RsolverVisionTest, CaptureImageFromSensor)
{
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
	m_rsolverVision->CalibrateCubeCameraDistance(true);
}


TEST_F(RsolverVisionTest, DISABLED_WriteCubiesToDisk)
{
	// Read from the disk
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

TEST_F(RsolverVisionTest, DISABLED_histogramTest)
{
	// Read from the disk
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		for (auto j = 0; j < g_cubiesPerFace; j++)
		{
			cv::Mat cubieImage = m_rsolverVision->GetCubieAtIndex(img, j);
			m_rsolverVision->DetectColorOfCubie(cubieImage);
		}
	}
}

TEST_F(RsolverVisionTest, CalibrateColorBoundariesTest)
{
	// Read from the disk
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		m_rsolverVision->CalibrateBoundariesByFaceColor(img, static_cast<Colors>(i));
	}

	// Read from the disk
	CubeStateInColors csInColors;
	for (int i = 0; i < 6; i++)
	{
		std::string inputFileName = "cube_" + std::to_string(i) + ".png";
		cv::Mat img = cv::imread(inputFileName);
		auto cubeFaceInfo = m_rsolverVision->GetCubeFaceInfoColorsFromImage(img);
		csInColors.emplace_back(cubeFaceInfo);
	}
}

}
}
