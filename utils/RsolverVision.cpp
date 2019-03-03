#pragma once
#include "RsolverVision.h"

namespace Rsolver {
	RsolverVision::RsolverVision()
		:m_width(g_defaultWidth)
		,m_height(g_defaultHeight)
	{
		SetCaptureDimensions(m_width, m_height);
	}

	cv::Mat RsolverVision::CaptureImageFromSensor()
	{
		cv::Mat image;
		m_videoCapture.open(0);
		if (m_videoCapture.isOpened())
		{
			m_videoCapture >> image;
			if (!image.empty())
			{
				return image;
			}
			throw(std::runtime_error("Retrieved image from sensor is empty!"));
		}
		throw(std::runtime_error("Unable to open video capture"));
	}

	cv::Mat RsolverVision::GetImageFromFile(std::string fileName)
	{
		return cv::Mat();
	}

	CubeStateInColors RsolverVision::GetCubeStateInColorsFromImage(const cv::Mat & image)
	{
		return CubeStateInColors();
	}

	void RsolverVision::SetCaptureDimensions(int width, int height)
	{
		m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, m_width);
		m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, m_height);
	}
}

