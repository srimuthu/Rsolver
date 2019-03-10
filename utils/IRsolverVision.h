#pragma once
#include "RsolverTypes.h"
#include "opencv2/core.hpp"

namespace Rsolver {

class IRsolverVision
{
	public:
	virtual ~IRsolverVision() = default;

	virtual cv::Mat CaptureImageFromSensor() = 0;
	virtual CubeFaceInfo GetCubeFaceInfoColorsFromImage(const cv::Mat& image) = 0;
	virtual void CalibrateCubeCameraDistanceGUI(bool testCapture) =  0;
	virtual void PerformBoundariesCalibrationByFaceColor(const cv::Mat& faceImage, Colors color) = 0;
};
}

