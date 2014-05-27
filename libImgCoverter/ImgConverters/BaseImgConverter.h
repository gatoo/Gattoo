#pragma once

#include "..\Interfaces\iimgconverter.h"

#define MAKE_LE(shY)  (((shY >> 8) & 0xFF) | ((shY << 8) & 0xFF00 ))
#define MAKE_DWORD_LE(dwY)  (((dwY >> 24) & 0x000000FF) | ((dwY >> 8) & 0x0000FF00) | ((dwY << 24) & 0xFF000000) | ((dwY << 8) & 0x00FF0000))

class CBaseImgConverter : public IImgConverter
{
public:

	CBaseImgConverter(void);
	virtual ~CBaseImgConverter(void);

	static void CreateBitmap(char const * const szInImgFile);

protected:

	struct SImgHeader
	{
		char szImgName[16];
		char szImgSize[16];

		short shXSize;
		short shYSize;

		unsigned int uiDotsCount;

		char padding[24];
	};

	static const int BLOCK_SIZE = 64;

	static char getLevel(uchar intencity);
	static unsigned char getColor(unsigned char chLevel);
};