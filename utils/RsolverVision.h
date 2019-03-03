#pragma once
#include "IRsolverVision.h"
#include "opencv2/videoio.hpp"

namespace Rsolver {

const int g_defaultWidth = 640;
const int g_defaultHeight = 480;

class RsolverVision : public IRsolverVision
{
	public:
		explicit RsolverVision();
		~RsolverVision() = default;

		virtual cv::Mat CaptureImageFromSensor() override;
		virtual cv::Mat GetImageFromFile(std::string fileName) override;
		virtual CubeStateInColors GetCubeStateInColorsFromImage(const cv::Mat& image) override;

	private:
		void SetCaptureDimensions(int width, int height);

		int						m_width;
		int						m_height;
		cv::VideoCapture		m_videoCapture;
};
}

