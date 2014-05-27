#pragma once

#include <opencv2/opencv.hpp>

enum EImgConverter
{
	enDefault
};

class IImgConverter
{
public:

	virtual bool Convert(cv::Mat const & img, FILE *fOut) = 0;
};

IImgConverter* createConverter(EImgConverter enType);