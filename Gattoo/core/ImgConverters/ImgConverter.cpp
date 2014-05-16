#include "stdafx.h"
#include "ImgConverter.h"

const int IImgConverter::BLOCK_SIZE = 64;

CImgConverter::CImgConverter(void)
{
}

CImgConverter::~CImgConverter(void)
{
}

bool CImgConverter::Convert(cv::Mat const & img, FILE *fOut)
{
	if (nullptr == fOut) return false;

	std::map<uchar, uchar> levels;
	
	uchar intensity;

	SImgHeader header;
	char szStrBuf[MAX_PATH];

	memset(&header, 0, sizeof(header));

	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", img.cols, img.rows);
	memset(header.szImgName, 0x20, sizeof(header.szImgName));
	memcpy(header.szImgName, szStrBuf, strlen(szStrBuf));

	double const dXScale = 0.239;
	double const dYScale = 0.237;

	int iFileSize = sizeof(header);
	char chPadding = (char) 0xFF;

	header.shXSize = img.cols;
	header.shYSize = img.rows;

	sprintf(szStrBuf, "%.1fx%.1fÏÏ", header.shXSize * dXScale, header.shYSize * dYScale);
	memset(header.szImgSize, 0x20, sizeof(header.szImgSize));
	memcpy(header.szImgSize, szStrBuf, strlen(szStrBuf));

	header.shXSize = MAKE_LE(header.shXSize);
	header.shYSize = MAKE_LE(header.shYSize);

	fwrite(&header, sizeof(header), 1, fOut);

	ushort shY = 0;
	ushort shX = 0;

	bool bFirstInLine = true;
	bool bReverse = false;
	bool bDot = false;

	size_t nLastDotPos = 0;

	std::vector<ushort> arrData;
	int const nOff = 10;

	int iLastDot = 0;

	for (int j=0; j<img.rows; j++)
	{
		for (int i = bReverse ? (img.cols-1) : 0; bReverse ? i >= 0 : i<img.cols; bReverse ? --i : ++i)
		{
			intensity = getLevel(img.at<uchar>(j, i));

			if (intensity == 7)
			{
				bDot = true;

				if (bFirstInLine)
				{

					int iDiff = bReverse ? (i - iLastDot) : (iLastDot - i);

					if (iDiff > 0)
					{
						for(int k = 1; k<=iDiff; k++)
						{
							shX = 0x0FFF & (bReverse ?  (iLastDot+k) : (iLastDot-k));
							shX = MAKE_LE(shX);

							arrData.push_back(shX);
						}
					}

					shY = 0x8000 | (0x0FFF & j);
					shY = MAKE_LE(shY);

					arrData.push_back(shY);
					bFirstInLine = false;

					iDiff = bReverse ? (iLastDot - i) : (i - iLastDot);

					if (iDiff > 0 && nLastDotPos)
					{
						for(int k = 0; k<iDiff; k++)
						{
							shX = 0x0FFF & (bReverse ? (iLastDot-k) : (iLastDot+k));
							shX = MAKE_LE(shX);

							arrData.push_back(shX);
						}
					}
				}

				shX = (0x7000 & (intensity << 12)) | (0x0FFF & i);
				shX = MAKE_LE(shX);

				arrData.push_back(shX);
				nLastDotPos = arrData.size();
				iLastDot = i;
			}
			else if (bDot) // Write empty dot
			{
				shX = 0x0FFF & i;
				shX = MAKE_LE(shX);

				arrData.push_back(shX);
			}
		}

		if (arrData.size())
		{
			header.uiDotsCount += (nLastDotPos-1);

			iFileSize += fwrite(&arrData[0], 1, nLastDotPos*sizeof(arrData[0]), fOut);
			arrData.clear();
		}

		if(!bFirstInLine)
			bReverse = !bReverse;

		bFirstInLine = true;
		bDot = false;
	}

	for (int i = BLOCK_SIZE - (iFileSize % BLOCK_SIZE); i > 0; --i)
		iFileSize += fwrite(&chPadding, 1, sizeof(char), fOut);

	// Update dots count
	header.uiDotsCount = MAKE_DWORD_LE(header.uiDotsCount);
	fseek(fOut, SEEK_SET, 0);
	fwrite(&header, sizeof(header), 1, fOut);

	fclose(fOut);

	return true;
}

char CImgConverter::getLevel(uchar intencity)
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

unsigned char CImgConverter::getColor(unsigned char chLevel)
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

void CImgConverter::CreateBitmap(char const * const szInImgFile)
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
