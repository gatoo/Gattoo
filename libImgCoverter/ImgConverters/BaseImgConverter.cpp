#include "stdafx.h"
#include "BaseImgConverter.h"

CBaseImgConverter::CBaseImgConverter(void)
{

}

CBaseImgConverter::~CBaseImgConverter(void)
{

}

void CBaseImgConverter::CreateBitmap(char const * const szInImgFile)
{
	cv::Mat srcImg;
	size_t readData = 0;
	SImgHeader header;

	char szDataBuf[BLOCK_SIZE];

	FILE *f = fopen(szInImgFile, "rb");
	if (NULL == f) return;

	readData = fread(&header, 1, sizeof(SImgHeader), f);

	if (sizeof(SImgHeader) != readData) return;

	srcImg.create(MAKE_LE(header.shYSize), MAKE_LE(header.shXSize), CV_8UC1);
	srcImg = 0x5F;

	readData = fread(szDataBuf, 1, BLOCK_SIZE, f);

	if (EOF == readData || readData != BLOCK_SIZE) return;

	unsigned short iY =0;
	unsigned short iX =0;
	unsigned char uColor = 0;
	unsigned short iData = 0;

	short *pShortData = (short*) szDataBuf;

	do 
	{
		for (int i=0; i<BLOCK_SIZE/sizeof(iData); ++i)
		{

			iData = MAKE_LE(pShortData[i]);

			if (iData & 0x8000)
			{
				iY = iData & 0xFFF;
				continue;
			}
			else
			{
				uColor = getColor((iData & 0x7000) >> 12);
				iX = iData & 0xFFF;
			}

			srcImg.at<unsigned char>(iY, iX) = uColor;
		}

		readData = fread(szDataBuf, 1, BLOCK_SIZE, f);

	} while (EOF != readData && readData == BLOCK_SIZE);

	std::string strOutName(szInImgFile);

	strOutName.erase(strOutName.rfind("."));
	strOutName.append(".bmp");

	cv::imwrite(strOutName, srcImg);
}

char CBaseImgConverter::getLevel(uchar intencity)
{
	if (intencity < 0x20)
		return 0;
	else if (intencity < 0x40)
		return 1;
	else if (intencity < 0x60)
		return 2;
	else if (intencity < 0x80)
		return 3;
	else if (intencity < 0xA0)
		return 4;
	else if (intencity < 0xC0)
		return 5;
	else if (intencity < 0xE0)
		return 6;
	else
		return 7;
}

unsigned char CBaseImgConverter::getColor(unsigned char chLevel)
{
	unsigned char chRes = 0;

	switch (chLevel)
	{
	case 7: chRes = 0xFF; break;
	case 6: chRes = 0xD8; break;
	case 5: chRes = 0xB4; break;
	case 4: chRes = 0x90; break;
	case 3: chRes = 0x6C; break;
	case 2: chRes = 0x48; break;
	case 1: chRes = 0x24; break;
	case 0: chRes = 0x00; break;

	default: assert(false); break;
	}

	return chRes;
}

void CBaseImgConverter::Destroy()
{
	delete this;
}
