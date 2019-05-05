#pragma once
#include "IRsolverVision.h"
#include "opencv2/opencv.hpp"

namespace Rsolver {

const int g_defaultWidth = 640;
const int g_defaultHeight = 480;
const int g_centerCubieIndex = 4;
const int g_histTolerance = 20;

struct CubeOverlayDefaults
{
	CubeOverlayDefaults()
		: xPosTopLeft(140)
		, yPosTopLeft(60)
		, cubieSizeInPix(120)
		, toleranceInPix(80)
	{}
	int xPosTopLeft;
	int yPosTopLeft;
	int cubieSizeInPix;
	int toleranceInPix;
};

class RsolverVision : public IRsolverVision
{
	public:
		explicit RsolverVision(int deviceId);
		~RsolverVision();

		virtual cv::Mat CaptureImageFromSensor() override;
		virtual CubeFaceInfo GetCubeFaceInfoColorsFromImage(const cv::Mat& image) override;
		virtual void CalibrateCubeCameraDistanceGUI(bool testCapture) override;
		virtual void PerformBoundariesCalibrationByFaceColor(const cv::Mat& faceImage, Colors color) override;

		// Non-interface public methods
		std::vector<ColorBoundaries> GetColorBoundariesVector();
		cv::Mat GetCubieAtIndex(const cv::Mat& inputImage, int index);
		Colors DetectColorOfCubie(const cv::Mat& cubie);

	private:
		void OpenVideoCapture();
		void SetCaptureDimensions(int width, int height);
		void CreateCubiesPositions();
		void OverlayCubiesExpectedPositionOnImage(cv::Mat& image);
		void InitializeDefaultColorBoundaries();
		LabVal GetLabValFromCubie(const cv::Mat& image);

		int										m_deviceId;
		int										m_width;
		int										m_height;
		cv::VideoCapture						m_videoCapture;
		std::vector<cv::Rect>					m_cubiesPos;
		std::vector<ColorBoundaries>			m_colorBoundariesVec;
};
}

