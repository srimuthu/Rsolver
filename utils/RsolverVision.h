#pragma once
#include "IRsolverVision.h"
#include "opencv2/videoio.hpp"
#include "opencv2/opencv.hpp"

namespace Rsolver {

const int g_defaultWidth = 640;
const int g_defaultHeight = 480;

struct CubeOverlayDefaults
{
	CubeOverlayDefaults()
		: xPosTopLeft(140)
		, yPosTopLeft(60)
		, cubieSizeInPix(120)
		, toleranceInPix(60)
	{}
	int xPosTopLeft;
	int yPosTopLeft;
	int cubieSizeInPix;
	int toleranceInPix;
};

class RsolverVision : public IRsolverVision
{
	public:
		explicit RsolverVision();
		~RsolverVision();

		virtual cv::Mat CaptureImageFromSensor() override;
		virtual CubeStateInColors GetCubeStateInColorsFromImage(const cv::Mat& image) override;
		virtual void CalibrateCubeCameraDistance(bool testCapture) override;

	private:
		void OpenVideoCapture();
		void SetCaptureDimensions(int width, int height);
		void OverlayCubiesExpectedPositionOnImage(cv::Mat& image);


		int						m_width;
		int						m_height;
		cv::VideoCapture		m_videoCapture;
};
}

