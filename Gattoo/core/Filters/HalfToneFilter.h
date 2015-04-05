#pragma once

#include "iimgfilter.h"

class CHalfToneFilter : public IImgFilter
{
public:
	CHalfToneFilter(void);
	virtual ~CHalfToneFilter(void);

	virtual bool Apply(cv::Mat & img);

protected:

	__forceinline void Trans(int Error, UCHAR & chData, double dFraction);

	__forceinline BYTE getNearestColor(BYTE byColor, std::vector<BYTE>& palette);

	void generatePalette(std::vector<BYTE>& palette, int paletteSize);
};

