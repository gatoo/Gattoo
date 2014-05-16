#pragma once

#include <opencv2/opencv.hpp>

#define MAKE_LE(shY)  (((shY >> 8) & 0xFF) | ((shY << 8) & 0xFF00 ))
#define MAKE_DWORD_LE(dwY)  (((dwY >> 24) & 0x000000FF) | ((dwY >> 8) & 0x0000FF00) | ((dwY << 24) & 0xFF000000) | ((dwY << 8) & 0x00FF0000))

class IImgConverter
{

public:

	struct SImgHeader
	{
		char szImgName[16];
		char szImgSize[16];

		short shXSize;
		short shYSize;

		unsigned int uiDotsCount;

		char padding[24];
	};

	static const int BLOCK_SIZE;

	virtual bool Convert(cv::Mat const & img, FILE *fOut) = 0;
};