#pragma once

#include <opencv2/opencv.hpp>

class IImgConverter
{
public:

	virtual bool Convert(cv::Mat const & img, FILE* fOut) = 0;
	virtual void Destroy() = 0;

	enum EImgConverter
	{
		enDefault
	};
};

IImgConverter* createImgConverter(IImgConverter::EImgConverter enType);
