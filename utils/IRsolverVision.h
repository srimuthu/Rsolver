#pragma once
#include "RsolverTypes.h"
#include "opencv2/core.hpp"

namespace Rsolver {

class IRsolverVision
{
	public:
	virtual ~IRsolverVision() = default;
	
	virtual cv::Mat CaptureImageFromSensor() = 0;
	virtual cv::Mat GetImageFromFile(std::string fileName) = 0;
	virtual CubeStateInColors GetCubeStateInColorsFromImage(const cv::Mat& image) = 0;

};
}

