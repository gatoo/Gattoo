#include "stdafx.h"
#include "ImgConverter.h"

#include <GlobalSettings.h>

CImgConverter::CImgConverter(void)
{
}

CImgConverter::~CImgConverter(void)
{
}

bool CImgConverter::Convert(cv::Mat const & img, FILE *fOut)
{
	if (nullptr == fOut) return false;

	uchar intensity;

	SImgHeader header;
	char szStrBuf[MAX_PATH];

	memset(&header, 0, sizeof(header));

	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", img.cols, img.rows);
	memset(header.szImgName, 0x20, sizeof(header.szImgName));
	memcpy(header.szImgName, szStrBuf, strlen(szStrBuf));

	int iFileSize = sizeof(header);
	unsigned char const uchPadding = static_cast<unsigned char>(0xFF);

	header.shXSize = img.cols;
	header.shYSize = img.rows;

	sprintf(szStrBuf, "%.1fx%.1fÏÏ", header.shXSize * CStaticSettings::HZ_SIZE_SCALE, header.shYSize * CStaticSettings::VT_SIZE_SCALE);
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
		iFileSize += fwrite(&uchPadding, 1, sizeof(char), fOut);

	// Update dots count
	header.uiDotsCount = MAKE_DWORD_LE(header.uiDotsCount);
	fseek(fOut, SEEK_SET, 0);
	fwrite(&header, sizeof(header), 1, fOut);

	fclose(fOut);

	return true;
}
