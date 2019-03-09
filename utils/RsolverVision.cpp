#include "RsolverVision.h"
#include "RsolverHelpers.h"

namespace Rsolver {
	RsolverVision::RsolverVision()
		:m_width(g_defaultWidth)
		,m_height(g_defaultHeight)
	{
		SetCaptureDimensions(m_width, m_height);
		CreateCubiesPositions();
		InitializeDefaultColorBoundaries();
	}

	RsolverVision::~RsolverVision()
	{
		m_videoCapture.release();
	}

	cv::Mat RsolverVision::CaptureImageFromSensor()
	{
		cv::Mat image;
		OpenVideoCapture();
		m_videoCapture >> image;
		if (image.empty())
		{
			throw(std::runtime_error("Retrieved image from sensor is empty!"));
		}
		m_videoCapture.release();
		return image;

	}

	CubeFaceInfo RsolverVision::GetCubeFaceInfoColorsFromImage(const cv::Mat & faceImage)
	{
		CubeFaceInfo cubeFaceInfo;
		for (auto i = 0; i < g_cubiesPerFace; i++)
		{
			cv::Mat cubie = GetCubieAtIndex(faceImage, i);
			cubeFaceInfo.faceColorVector.emplace_back(DetectColorOfCubie(cubie));
		}
		cubeFaceInfo.cubeFace = Helpers::GetDefaultCubeFaceForColor(cubeFaceInfo.faceColorVector.at(g_centerCubieIndex));
		return cubeFaceInfo;
	}

	void RsolverVision::CalibrateCubeCameraDistance(bool testCapture)
	{
		cv::Mat originalFrame, overlayFrame;
		int keycount = 0;
		OpenVideoCapture();
		while (true)
		{
			m_videoCapture >> originalFrame;
			if (originalFrame.empty())
			{
				throw(std::runtime_error("Retrieved image from sensor is empty!"));
			}
			originalFrame.copyTo(overlayFrame);
			OverlayCubiesExpectedPositionOnImage(overlayFrame);
			cv::imshow("Live", overlayFrame);
			int key = cv::waitKey(5);
			key = (key == 255) ? -1 : key;
			if (key >= 0)
			{
				if (testCapture)
				{
					std::string filename = "cube_" + std::to_string(keycount) + ".png";
					keycount++;
					std::cout << "capturing" << filename << std::endl;
					cv::imwrite(filename, originalFrame);
				}
				else
				{
					keycount = g_numFaces;
				}
				if (keycount >= g_numFaces)	break;
			}
		}
		m_videoCapture.release();
	}

	void RsolverVision::OpenVideoCapture()
	{
		m_videoCapture.open(0);
		if (!m_videoCapture.isOpened())
		{
			throw(std::runtime_error("Unable to open video capture"));
		}
	}

	void RsolverVision::SetCaptureDimensions(int width, int height)
	{
		m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, width);
		m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, height);
	}

	void RsolverVision::CreateCubiesPositions()
	{
		auto overlay = CubeOverlayDefaults();
		cv::Rect cubie;
		for (auto i = 0; i < 3; i++)
		{
			for (auto j = 0; j < 3; j++)
			{
				cubie = cv::Rect(overlay.xPosTopLeft + (j*overlay.cubieSizeInPix) + (overlay.toleranceInPix / 2),
					overlay.yPosTopLeft + (i*overlay.cubieSizeInPix) + (overlay.toleranceInPix / 2),
					overlay.cubieSizeInPix - overlay.toleranceInPix,
					overlay.cubieSizeInPix - overlay.toleranceInPix);
				m_cubiesPos.emplace_back(cubie);
			}
		}
	}

	void RsolverVision::OverlayCubiesExpectedPositionOnImage(cv::Mat & image)
	{
		for (auto const& cubie : m_cubiesPos)
		{
			cv::rectangle(image, cubie, cv::Scalar(255, 0, 0), 4);
		}
	}

	void RsolverVision::InitializeDefaultColorBoundaries()
	{
		for (auto i = 0; i < g_numFaces; i++)
		{
			ColorBoundaries colorBoundaries(static_cast<Colors>(i));
			m_colorBoundariesVec.emplace_back(colorBoundaries);
		}
	}

	BgrVal RsolverVision::GetbgrValFromCubie(const cv::Mat & image)
	{
		std::vector<cv::Mat> bgrPlanes(3);
		cv::split(image, bgrPlanes);

		int histSize = 256;
		float range[] = { 0, 256 };
		const float* histRange = { range };
		bool uniform = true, accumulate = false;

		cv::Mat bHist, gHist, rHist;
		cv::calcHist(&bgrPlanes[0], 1, 0, cv::Mat(), bHist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgrPlanes[1], 1, 0, cv::Mat(), gHist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgrPlanes[2], 1, 0, cv::Mat(), rHist, 1, &histSize, &histRange, uniform, accumulate);
		BgrVal bgrVal;
		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(bHist, &minVal, &maxVal, &minLoc, &maxLoc);
		bgrVal.bVal = maxLoc.y;
		cv::minMaxLoc(gHist, &minVal, &maxVal, &minLoc, &maxLoc);
		bgrVal.gVal = maxLoc.y;
		cv::minMaxLoc(rHist, &minVal, &maxVal, &minLoc, &maxLoc);
		bgrVal.rVal = maxLoc.y;

		return bgrVal;
	}

	std::vector<ColorBoundaries> RsolverVision::GetColorBoundariesVector()
	{
		return m_colorBoundariesVec;
	}

	cv::Mat RsolverVision::GetCubieAtIndex(const cv::Mat& inputImage, int index)
	{
		if (index >= g_cubiesPerFace)
		{
			throw(std::runtime_error("Invalid cubie index, cubie index must be less than " + std::to_string(g_cubiesPerFace)));
		}
		cv::Mat copyOfInputImage;
		inputImage.copyTo(copyOfInputImage);
		cv::Mat croppedCubie = copyOfInputImage(m_cubiesPos.at(index));
		cv::Mat croppedCubieGB;
		cv::GaussianBlur(croppedCubie, croppedCubieGB, cv::Size(7, 7), 0, 0);
		return croppedCubieGB;
	}

	Colors RsolverVision::DetectColorOfCubie(const cv::Mat & cubie)
	{
		BgrVal bgrVal = GetbgrValFromCubie(cubie);
		for (auto const& cb : m_colorBoundariesVec)
		{
			if ((bgrVal.bVal <= (cb.bgrVal.bVal + g_histTolerance)) && (bgrVal.bVal >= (cb.bgrVal.bVal - g_histTolerance)) &&
				(bgrVal.gVal <= (cb.bgrVal.gVal + g_histTolerance)) && (bgrVal.gVal >= (cb.bgrVal.gVal - g_histTolerance)) &&
				(bgrVal.rVal <= (cb.bgrVal.rVal + g_histTolerance)) && (bgrVal.rVal >= (cb.bgrVal.rVal - g_histTolerance)))
			{
				return cb.color;
			}
		}
		throw(std::runtime_error("Unable to identify the color of the cubie! Try recalibration"));
	}

	void RsolverVision::CalibrateBoundariesByFaceColor(const cv::Mat & faceImage, Colors color)
	{
		cv::Mat centerCubie = GetCubieAtIndex(faceImage, g_centerCubieIndex);
		// Given the face color (of the center cubie) calibrate boundaries for that color
		for (auto& cb : m_colorBoundariesVec)
		{
			if (cb.color == color)
			{
				cb.bgrVal = GetbgrValFromCubie(centerCubie);
			}
		}
	}
}

