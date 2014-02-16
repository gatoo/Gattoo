#pragma once

#include "iimgfilter.h"

class CEqulizeFilter : public IImgFilter
{

public:
	CEqulizeFilter(void);
	virtual ~CEqulizeFilter(void);

	virtual bool Apply(cv::Mat & img);
};

