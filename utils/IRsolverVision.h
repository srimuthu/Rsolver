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
	virtual void CalibrateCubeCameraDistance(bool testCapture) =  0;
};
}

