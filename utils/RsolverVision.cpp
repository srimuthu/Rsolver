#include "RsolverVision.h"

namespace Rsolver {
	RsolverVision::RsolverVision()
		:m_width(g_defaultWidth)
		,m_height(g_defaultHeight)
	{
		SetCaptureDimensions(m_width, m_height);
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

	CubeStateInColors RsolverVision::GetCubeStateInColorsFromImage(const cv::Mat & image)
	{
		return CubeStateInColors();
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
	void RsolverVision::OverlayCubiesExpectedPositionOnImage(cv::Mat & image)
	{
		auto overlay = CubeOverlayDefaults();
		cv::Rect cubie;
		for (auto i = 0; i < 3; i++)
		{
			for (auto j = 0; j < 3; j++)
			{
				cubie = cv::Rect(overlay.xPosTopLeft + (i*overlay.cubieSizeInPix) + (overlay.toleranceInPix/2), 
								overlay.yPosTopLeft + (j*overlay.cubieSizeInPix) + (overlay.toleranceInPix /2),
								overlay.cubieSizeInPix - overlay.toleranceInPix, 
								overlay.cubieSizeInPix - overlay.toleranceInPix);
				cv::rectangle(image, cubie, cv::Scalar(255, 0, 0), 4);
			}
		}
	}
}

