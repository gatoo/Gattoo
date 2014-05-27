#pragma once

#include "BaseImgConverter.h"

class CImgConverter : public CBaseImgConverter
{
public:

	CImgConverter(void);
	virtual ~CImgConverter(void);

	virtual bool Convert(cv::Mat const & img, FILE *fOut);
};

