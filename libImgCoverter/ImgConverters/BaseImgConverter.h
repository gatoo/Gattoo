#pragma once

#include "..\Interfaces\iimgconverter.h"

class CBaseImgConverter : public IImgConverter
{
public:

	CBaseImgConverter(void);
	virtual ~CBaseImgConverter(void);

	bool CreateBitmap(char const * const szInImgFile);

	virtual void Destroy();

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