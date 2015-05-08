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

	const int iPaletteSize = 8;

	std::vector<BYTE> palette(iPaletteSize);

	generatePalette(palette, iPaletteSize);

	int Error = 0;

	//std::map<UCHAR, int> colors;

	UCHAR* p = nullptr, *p2 = nullptr;

	for (int i = 0; i < img.rows - 1; i++)
	{
		p = img.ptr<UCHAR>(i);
		p2 = img.ptr<UCHAR>(i + 1);
		
		p[0] = 0;

		for (int j = 1; j < img.cols - 1; j++)
		{
			BYTE intencity = p[j];
			BYTE newIntencity = getNearestColor(intencity, palette);

			p[j] = newIntencity;

			//colors[newIntencity]++;
			Error = intencity - newIntencity;
			
			Trans(Error, p[j + 1], 7.0 / 16);
			Trans(Error, p2[j + 1], 1.0 / 16);
			Trans(Error, p2[j], 5.0 / 16);
			Trans(Error, p2[j - 1], 3.0 / 16);
			
		}
		
		p[img.cols - 1] = 0;
	}

	// Fill last row
	p = img.ptr<UCHAR>(img.rows - 1);
	memset(p, 0, img.cols);

	return true;
}

__forceinline void CHalfToneFilter::Trans(int Error, UCHAR & chData, double dFraction)
{
	chData = std::min(255, std::max(0, static_cast<int>(chData + dFraction * Error)));
	//chData = chData + dFraction * Error;
}

__forceinline BYTE CHalfToneFilter::getNearestColor(BYTE byColor, std::vector<BYTE>& palette)
{
	int distanceSquared, minDistanceSquared, bestIndex = 0;
	minDistanceSquared = 255 * 255 + 1;

	for (size_t i = 0; i < palette.size(); i++)
	{
		int diff = byColor - palette[i];
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
	double paletteWidth = static_cast<double> (0xFF) / (paletteSize - 1);

	palette[0] = 0xFF;

	for (int i = 1; i < paletteSize; i++)
	{
		palette[i] = static_cast<UCHAR>(0xFF - paletteWidth * i);
	}
}
