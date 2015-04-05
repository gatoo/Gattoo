#include "stdafx.h"
#include "HalfToneFilter.h"
#include <vector>

CHalfToneFilter::CHalfToneFilter(void)
{
}

CHalfToneFilter::~CHalfToneFilter(void)
{
}

bool CHalfToneFilter::Apply(cv::Mat & img)
{
	if (img.empty()) return true;

	const int iPaletteSize = 3;
	std::vector<BYTE> palette(iPaletteSize);

	generatePalette(palette, iPaletteSize);

	int Error = 0;

	//int n=0;

	for(int i=0; i<img.rows-1; i++)
		for (int j=1; j<img.cols-1; j++)
		{
			BYTE intencity = *img.ptr<UCHAR>(i, j);

			BYTE newIntencity = getNearestColor(intencity, palette);
			memset(img.ptr<UCHAR>(i, j), newIntencity, 1);
			Error = intencity - newIntencity;
			
			/*if(intencity > 128)
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
			*/

			Trans(Error, *img.ptr<UCHAR>(i, j+1), (double)7/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j+1), (double)1/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j), (double)5/16);
			Trans(Error, *img.ptr<UCHAR>(i+1, j-1), (double)3/16);
		}

	return true;
}

__forceinline void CHalfToneFilter::Trans(int Error, UCHAR & chData, double dFraction)
{
	chData = std::min(255, std::max(0, chData + int(dFraction * Error)));
}

__forceinline BYTE CHalfToneFilter::getNearestColor(BYTE byColor, std::vector<BYTE>& palette)
{
	int distanceSquared, minDistanceSquared, bestIndex = 0;
	minDistanceSquared = 255 * 255 + 1;
	
	for (int i = 0; i < palette.size(); i++)
	{
		int diff = ((int)byColor) - palette[i];
		distanceSquared = diff*diff;
		
		if (distanceSquared < minDistanceSquared)
		{
			minDistanceSquared = distanceSquared;
			bestIndex = i;
		}
	}
	
	return palette[bestIndex];
}

void CHalfToneFilter::generatePalette(std::vector<BYTE>& palette, int paletteSize)
{
	double paletteWidth = (double) 0xFF / (paletteSize - 1);
	
	palette[0] = 0xFF;

	for (int i = 1; i < paletteSize; i++)
	{
		palette[i] = 0xFF - paletteWidth * i;
	}
}
