#pragma once

#include <opencv2/opencv.hpp>

class IImgFilter
{
public:

	virtual bool Apply(cv::Mat & img) = 0;
};