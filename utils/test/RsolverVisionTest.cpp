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

	std::unique_ptr<IRsolverVision>	m_rsolverVision;
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

TEST_F(RsolverVisionTest, GetImagesForTesting)
{
	m_rsolverVision->CalibrateCubeCameraDistance(true);
}
}
}
