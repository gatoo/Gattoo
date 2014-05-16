#pragma once

#include "iimgconverter.h"

class CImgConverter : public IImgConverter
{
public:
	CImgConverter(void);
	virtual ~CImgConverter(void);

	virtual bool Convert(cv::Mat const & img, FILE *fOut);

	static void CreateBitmap(char const * const szInImgFile);

protected:

	static char getLevel(uchar intencity);
	static unsigned char getColor(unsigned char chLevel);
};

