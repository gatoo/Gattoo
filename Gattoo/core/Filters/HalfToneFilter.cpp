#include "stdafx.h"
#include "HalfToneFilter.h"

CHalfToneFilter::CHalfToneFilter(void)
{
}

CHalfToneFilter::~CHalfToneFilter(void)
{
}

bool CHalfToneFilter::Apply(cv::Mat & img)
{
	if (img.empty()) return true;

	int Error = 0;

	int n=0;

	for(int i=0; i<img.rows-1; i++)
		for (int j=1; j<img.cols-1; j++)
		{
			BYTE intencity = *img.ptr<UCHAR>(i, j);

			if(intencity > 128)
			{
				memset(img.ptr<UCHAR>(i, j), 0xFF, 1);
				Error = intencity - 0xFF;
				n++;
			}
			else
			{
				memset(img.ptr<UCHAR>(i, j), 0x00, 1) ;
				Error = intencity;
				n++;
			}

			Trans(Error, *img.ptr<UCHAR>(i, j+1), (double)7/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j+1), (double)1/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j), (double)5/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j-1), (double)3/16);
		}

#ifdef _DEBUG
	cv::imshow("Halftoned", img);
#endif

	return true;
}

__forceinline void CHalfToneFilter::Trans(int Error, UCHAR & chData, double dFraction)
{
	chData = std::min(255, std::max(0, chData + int(dFraction * Error)));
}