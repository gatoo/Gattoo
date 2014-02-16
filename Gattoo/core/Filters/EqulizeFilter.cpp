#include "stdafx.h"
#include "EqulizeFilter.h"

CEqulizeFilter::CEqulizeFilter(void)
{
}


CEqulizeFilter::~CEqulizeFilter(void)
{
}

bool CEqulizeFilter::Apply(cv::Mat &img)
{
	cv::cvtColor(img, img, CV_RGB2GRAY);

	/// Apply Histogram Equalization
	cv::equalizeHist(img, img);

	cv::cvtColor(img, img, CV_GRAY2RGB);

	return true;
}
