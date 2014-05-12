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
	if (img.empty()) return true;

	// Apply Histogram Equalization
	cv::equalizeHist(img, img);

// #ifdef _DEBUG
// 	cv::imshow("Equalized", img);
// #endif

	return true;
}
