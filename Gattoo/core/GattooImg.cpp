#include "stdafx.h"
#include "GattooImg.h"

#include "Filters/IImgFilter.h"
#include "Filters/EqulizeFilter.h"
#include "Filters/HalfToneFilter.h"

#include "ImgConverters/ImgConverter.h"

CGattooImg::CGattooImg(void)
{
}

CGattooImg::~CGattooImg(void)
{
}

/*
void CGattooImg::SaveImgData(unsigned short shXDim, unsigned short shYDim)
{
	char szStrBuf[MAX_PATH];

	memset(&m_header, 0, sizeof(m_header));

	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", shXDim, shYDim);
	memset(m_header.szImgName, 0x20, sizeof(m_header.szImgName));
	memcpy(m_header.szImgName, szStrBuf, strlen(szStrBuf));

	char chXSize = shXDim;
	char chYSize = shYDim;
	char chDelim = 5;

	double dXScale = 0.239;
	double dYScale = 0.237;

	int iFileSize = sizeof(m_header);
	char chPadding = 0xFF;

	m_header.shXSize = 8*(chXSize+chDelim);
	m_header.shYSize = chYSize;
	
	sprintf(szStrBuf, "%.1fx%.1fÏÏ", m_header.shXSize * dXScale, m_header.shYSize * dYScale);
	memset(m_header.szImgSize, 0x20, sizeof(m_header.szImgSize));
	memcpy(m_header.szImgSize, szStrBuf, strlen(szStrBuf));

	m_header.shXSize = MAKE_LE(m_header.shXSize);
	m_header.shYSize = MAKE_LE(m_header.shYSize);

	sprintf(szStrBuf, "%hux%hu.pix", shXDim, shYDim);

	FILE *fOut = NULL;
	fOut = fopen(szStrBuf, "wb");
	if (nullptr == fOut) return;

	fwrite(&m_header, sizeof(m_header), 1, fOut);

	unsigned short iStartX = 0;
	unsigned short iStartY = 0;

	unsigned short shY = 0;
	unsigned short shX = 0;

	unsigned short shXMask = 0;
	short iXOffset = 1;

	for (int j=0; j<chYSize; j++)
	{
		shY = 0x8000 | (0x0FFF & (iStartY + j));
		shY = MAKE_LE(shY);
		iFileSize += fwrite(&shY, 1, sizeof(shY), fOut);

		for(int k=0; k<8; ++k)
		{
			unsigned short shXMask = (0x7 & ( j % 2 ? ~k : k )) << 12;

			if (j % 2)
			{
				for (int i=0; i<chDelim; ++i)
				{
					iStartX --;
					shX = 0x7000 | (0x0FFF & (iStartX));
					shX = MAKE_LE(shX);
					iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);				
					
				}

				for (int i=0; i<chXSize; ++i)
				{
					iStartX --;
					shX = shXMask | (0x0FFF & (iStartX));
					shX = MAKE_LE(shX);
					iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);									
				}
			}
			else
			{
				for (int i=0; i<chXSize; ++i)
				{
					shX = shXMask | (0x0FFF & (iStartX));
					shX = MAKE_LE(shX);
					iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);				
					iStartX ++;
				}

				for (int i=0; i<chDelim; ++i)
				{
					shX = 0x7000 | (0x0FFF & (iStartX));
					shX = MAKE_LE(shX);
					iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);				
					iStartX ++;
				}
			}
			
		}
	}	
	
	if (iFileSize % 64)
	{
		for (int i = 64 - (iFileSize % 64); i > 0; --i)
			iFileSize += fwrite(&chPadding, 1, sizeof(char), fOut);
	}

	fclose(fOut);

	CGattooImg::CreateBitmap(szStrBuf);
}
*/

// void CGattooImg::CreateBitmap(char const * const szInImgFile)
// {
// 	cv::Mat srcImg;
// 	size_t readData = 0;
// 	SImgHeader header;
// 
// 	char szDataBuf[BLOCK_SIZE];
// 
// 	FILE *f = fopen(szInImgFile, "rb");
// 	if (NULL == f) return;
// 
// 	readData = fread(&header, 1, sizeof(SImgHeader), f);
// 
// 	if (sizeof(SImgHeader) != readData) return;
// 
// 	srcImg.create(MAKE_LE(header.shYSize), MAKE_LE(header.shXSize), CV_8UC1);
// 	srcImg = 0x5F;
// 	
// 	readData = fread(szDataBuf, 1, BLOCK_SIZE, f);
// 
// 	if (EOF == readData || readData != BLOCK_SIZE) return;
// 
// 	unsigned short iY =0;
// 	unsigned short iX =0;
// 	unsigned char uColor = 0;
// 	unsigned short iData = 0;
// 
// 	short *pShortData = (short*) szDataBuf;
// 
// 	do 
// 	{
// 		for (int i=0; i<BLOCK_SIZE/sizeof(iData); ++i)
// 		{
// 
// 			iData = MAKE_LE(pShortData[i]);
// 
// 			if (iData & 0x8000)
// 			{
// 				iY = iData & 0xFFF;
// 				continue;
// 			}
// 			else
// 			{
// 				uColor = getColor((iData & 0x7000) >> 12);
// 				iX = iData & 0xFFF;
// 			}
// 
// 			srcImg.at<unsigned char>(iY, iX) = uColor;
// 		}
// 
// 		readData = fread(szDataBuf, 1, BLOCK_SIZE, f);
// 
// 	} while (EOF != readData && readData == BLOCK_SIZE);
// 
// 	std::string strOutName(szInImgFile);
// 
// 	strOutName.erase(strOutName.rfind("."));
// 	strOutName.append(".bmp");
// 
// 	cv::imwrite(strOutName, srcImg);
// }

unsigned char CGattooImg::getColor(unsigned char chLevel)
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

// void CGattooImg::ConvertBitmap(char const * const szInImgFile)
// {
// 	std::map<uchar, uchar> levels;
// 	cv::Mat srcImg;
// 
// 	srcImg = cv::imread(szInImgFile, CV_LOAD_IMAGE_GRAYSCALE);
// 
// 	uchar intensity;
// 
// /*	for (int j=0; j<srcImg.rows; j++)
// 		for (int i=0; i<srcImg.cols; ++i)
// 		{
// 			intensity = srcImg.at<uchar>(j, i);
// 			levels[intensity] = 0;
// 		}	
// 	
// 	if (levels.size() > 8) return;
// 
// 	uchar level = 0;
// 	for (std::map<uchar, uchar>::iterator iter = levels.begin(); iter != levels.end(); ++iter)
// 	{
// 		iter->second = level++;
// 	}*/
// 
// 	char szStrBuf[MAX_PATH];
// 	SImgHeader header;
// 
// 	memset(&header, 0, sizeof(header));
// 
// 	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", srcImg.cols, srcImg.rows);
// 	memset(header.szImgName, 0x20, sizeof(header.szImgName));
// 	memcpy(header.szImgName, szStrBuf, strlen(szStrBuf));
// 
// 	double dXScale = 0.239;
// 	double dYScale = 0.237;
// 
// 	int iFileSize = sizeof(header);
// 	char chPadding = (char) 0xFF;
// 
// 	header.shXSize = srcImg.cols;
// 	header.shYSize = srcImg.rows;
// 
// 	sprintf(szStrBuf, "%.1fx%.1fÏÏ", header.shXSize * dXScale, header.shYSize * dYScale);
// 	memset(header.szImgSize, 0x20, sizeof(header.szImgSize));
// 	memcpy(header.szImgSize, szStrBuf, strlen(szStrBuf));
// 
// 	header.shXSize = MAKE_LE(header.shXSize);
// 	header.shYSize = MAKE_LE(header.shYSize);
// 
// 	sprintf(szStrBuf, "%s.pix", szInImgFile);
// 
// 	FILE *fOut = NULL;
// 	fOut = fopen(szStrBuf, "wb");
// 	if (nullptr == fOut) return;
// 
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	ushort shY = 0;
// 	ushort shX = 0;
// 
// 	bool bFirstInLine = true;
// 	bool bReverse = false;
// 	bool bDot = false;
// 
// 	size_t nLastDotPos = 0;
// 
// 	std::vector<ushort> arrData;
// 	int const nOff = 10;
// 	
// 	for (int j=0; j<srcImg.rows; j++)
// 	{
// 		for (int i = bReverse ? (srcImg.cols-1) : 0; bReverse ? i >= 0 : i<srcImg.cols; bReverse ? --i : ++i)
// 		{
// 			intensity = getLevel(srcImg.at<uchar>(j, i));
// 			
// 			if (intensity)
// 			{
// 				bDot = true;
// 
// 				if (bFirstInLine)
// 				{
// 					shY = 0x8000 | (0x0FFF & j);
// 					shY = MAKE_LE(shY);
// 
// 					arrData.push_back(shY);
// 					//iFileSize +=  fwrite(&shY, 1, sizeof(shY), fOut);
// 					bFirstInLine = false;
// 
// 					// Add offset of n dots
// 					
// 					int iStart = bReverse ? std::min(srcImg.cols - 1, i + nOff) : std::max(0, i - nOff);
// 				
// 					for(int k = iStart; bReverse ? k>i : k < i; bReverse ? --k : ++k)
// 					{
// 						shX = (0x0FFF & k);
// 						shX = MAKE_LE(shX);
// 						arrData.push_back(shX);
// 						_ASSERTE(shX);
// 					}
// 				}
// 
// 				shX = (0x7000 & (intensity << 12)) | (0x0FFF & i);
// 				shX = MAKE_LE(shX);
// 
// 				//iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);
// 				arrData.push_back(shX);
// 				nLastDotPos = arrData.size();
// 			}
// 			else if (bDot) // Write empty dot
// 			{
// 				shX = 0x0FFF & i;
// 				shX = MAKE_LE(shX);
// 
// // 				if (bFirstInLine)
// // 				{
// // 					shY = 0x8000 | (0x0FFF & j);
// // 					shY = MAKE_LE(shY);
// // 
// // 					//iFileSize += fwrite(&shY, 1, sizeof(shY), fOut);
// // 					arrData.push_back(shY);
// // 					bFirstInLine = false;
// // 				}
// 
// 				// iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);
// 				arrData.push_back(shX);
// 			}
// 		}
// 
// 		if (arrData.size())
// 		{
// 			// Add offset of n dots
// 			int i = (0x0FFF & MAKE_LE(arrData[nLastDotPos-1]));
// 			int iStop = !bReverse ? std::min(srcImg.cols - 1, i + nOff) : std::max(0, i - nOff);
// 
// 			i +=  bReverse ? -1 : 1;
// 
// 			for(int k = i; bReverse ? k>= iStop : k <= iStop; bReverse ? --k : ++k)
// 			{
// 				shX = (0x0FFF & k);
// 				shX = MAKE_LE(shX);
// 				arrData.push_back(shX);
// 				nLastDotPos++;
// 
// 				_ASSERTE(shX);
// 			}
// 
// 			header.uiDotsCount += (nLastDotPos-1);
// 
// 			iFileSize += fwrite(&arrData[0], 1, nLastDotPos*sizeof(arrData[0]), fOut);
// 			arrData.clear();
// 		}
// 
// 		if(!bFirstInLine)
// 			bReverse = !bReverse;
// 
// 		bFirstInLine = true;
// 		bDot = false;
// 	}
// 
// 	for (int i = BLOCK_SIZE - (iFileSize % BLOCK_SIZE); i > 0; --i)
// 			iFileSize += fwrite(&chPadding, 1, sizeof(char), fOut);
// 
// 	// Update dots count
// 	header.uiDotsCount = MAKE_DWORD_LE(header.uiDotsCount);
// 	fseek(fOut, SEEK_SET, 0);
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	fclose(fOut);
// 
// 	CreateBitmap(szStrBuf);
// }

// void CGattooImg::ConvertBitmapWithMaxMin(char const * const szInImgFile)
// {
// 	std::map<uchar, uchar> levels;
// 	cv::Mat srcImg;
// 
// 	srcImg = cv::imread(szInImgFile, CV_LOAD_IMAGE_GRAYSCALE);
// 
// 	uchar intensity;
// 
// /*	for (int j=0; j<srcImg.rows; j++)
// 		for (int i=0; i<srcImg.cols; ++i)
// 		{
// 			intensity = srcImg.at<uchar>(j, i);
// 			levels[intensity] = 0;
// 		}	
// 	
// 	if (levels.size() > 8) return;
// 
// 	uchar level = 0;
// 	for (std::map<uchar, uchar>::iterator iter = levels.begin(); iter != levels.end(); ++iter)
// 	{
// 		iter->second = level++;
// 	}*/
// 
// 	char szStrBuf[MAX_PATH];
// 	SImgHeader header;
// 
// 	memset(&header, 0, sizeof(header));
// 
// 	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", srcImg.cols, srcImg.rows);
// 	memset(header.szImgName, 0x20, sizeof(header.szImgName));
// 	memcpy(header.szImgName, szStrBuf, strlen(szStrBuf));
// 
// 	double dXScale = 0.239;
// 	double dYScale = 0.237;
// 
// 	int iFileSize = sizeof(header);
// 	char chPadding = (char) 0xFF;
// 
// 	header.shXSize = srcImg.cols;
// 	header.shYSize = srcImg.rows;
// 
// 	sprintf(szStrBuf, "%.1fx%.1fÏÏ", header.shXSize * dXScale, header.shYSize * dYScale);
// 	memset(header.szImgSize, 0x20, sizeof(header.szImgSize));
// 	memcpy(header.szImgSize, szStrBuf, strlen(szStrBuf));
// 
// 	header.shXSize = MAKE_LE(header.shXSize);
// 	header.shYSize = MAKE_LE(header.shYSize);
// 
// 	sprintf(szStrBuf, "%s.pix", szInImgFile);
// 
// 	FILE *fOut = NULL;
// 	fOut = fopen(szStrBuf, "wb");
// 	if (nullptr == fOut) return;
// 
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	ushort shY = 0;
// 	ushort shX = 0;
// 
// 	bool bFirstInLine = true;
// 	bool bReverse = false;
// 	//bool bDot = false;
// 
// 	size_t nLastDotPos = 0;
// 
// 	std::vector<ushort> arrData;
// 	
// 	int iMaxX=0, iMaxY=0, iMinX=srcImg.cols, iMinY=srcImg.rows;
// 	getSideDots(srcImg, iMinX, iMaxX, iMinY, iMaxY);
// 
// 	for (int j=iMinY; j<=iMaxY; j++)
// 	{
// 		for (int i = bReverse ? (iMaxX) : iMinX; bReverse ? i >= iMinX : i<=iMaxX; bReverse ? --i : ++i)
// 		{
// 			intensity = getLevel(srcImg.at<uchar>(j, i));
// 			
// 			if (intensity)
// 			{
// 				//bDot = true;
// 
// 				header.uiDotsCount ++;
// 
// 				shX = (0x7000 & (intensity << 12)) | (0x0FFF & i);
// 				shX = MAKE_LE(shX);
// 				
// 				if (bFirstInLine)
// 				{
// 					shY = 0x8000 | (0x0FFF & j);
// 					shY = MAKE_LE(shY);
// 
// 					arrData.push_back(shY);
// 					//iFileSize +=  fwrite(&shY, 1, sizeof(shY), fOut);
// 					bFirstInLine = false;
// 				}
// 
// 				//iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);
// 				arrData.push_back(shX);
// 				//nLastDotPos = arrData.size();
// 			}
// 			else //if (bDot)// Write empty dot
// 			{
// 				header.uiDotsCount ++;
// 
// 				shX = 0x0FFF & i;
// 				shX = MAKE_LE(shX);
// 
// 				if (bFirstInLine)
// 				{
// 					shY = 0x8000 | (0x0FFF & j);
// 					shY = MAKE_LE(shY);
// 
// 					//iFileSize += fwrite(&shY, 1, sizeof(shY), fOut);
// 					arrData.push_back(shY);
// 					bFirstInLine = false;
// 				}
// 
// 				// iFileSize += fwrite(&shX, 1, sizeof(shX), fOut);
// 				arrData.push_back(shX);
// 			}
// 		}
// 
// 		if(!bFirstInLine)
// 			bReverse = !bReverse;
// 
// 		if (arrData.size())
// 		{
// 			iFileSize += fwrite(&arrData[0], 1, arrData.size()/*nLastDotPos*/*sizeof(arrData[0]), fOut);
// 			arrData.clear();
// 		}
// 
// 		bFirstInLine = true;
// 		//bDot = false;
// 	}
// 
// 	for (int i = BLOCK_SIZE - (iFileSize % BLOCK_SIZE); i > 0; --i)
// 			iFileSize += fwrite(&chPadding, 1, sizeof(char), fOut);
// 
// 	// Update dots count
// 	header.uiDotsCount = MAKE_DWORD_LE(header.uiDotsCount);
// 	fseek(fOut, SEEK_SET, 0);
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	fclose(fOut);
// 
// 	CreateBitmap(szStrBuf);
// }

char CGattooImg::getLevel(uchar intencity)
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

// void CGattooImg::getSideDots(cv::Mat &img, int &iMinX, int &iMaxX, int &iMinY, int &iMaxY)
// {
// 	bool bReverse = false;
// 
// 	for (int j=0; j<img.rows; j++)
// 	{
// 		for (int i = 0; i<img.cols; ++i)
// 		{
// 			if (getLevel(img.at<uchar>(j, i)))
// 			{
// 				iMinX = std::min(iMinX, i);
// 				iMaxX = std::max(iMaxX, i);
// 
// 				iMinY = std::min(iMinY, j);
// 				iMaxY = std::max(iMaxY, j);
// 			}
// 		}
// 	}
// }

// void CGattooImg::ConvertBitmapByColor(char const * const szInImgFile)
// {
// 	std::map<uchar, uchar> levels;
// 	cv::Mat srcImg;
// 
// 	srcImg = cv::imread(szInImgFile, CV_LOAD_IMAGE_GRAYSCALE);
// 
// 	uchar intensity;
// 
// 	char szStrBuf[MAX_PATH];
// 	SImgHeader header;
// 
// 	memset(&header, 0, sizeof(header));
// 
// 	sprintf(szStrBuf, "–»—”ÕŒ _%hux%hu", srcImg.cols, srcImg.rows);
// 	memset(header.szImgName, 0x20, sizeof(header.szImgName));
// 	memcpy(header.szImgName, szStrBuf, strlen(szStrBuf));
// 
// 	double dXScale = 0.239;
// 	double dYScale = 0.237;
// 
// 	int iFileSize = sizeof(header);
// 	char chPadding = (char) 0xFF;
// 
// 	header.shXSize = srcImg.cols;
// 	header.shYSize = srcImg.rows;
// 
// 	sprintf(szStrBuf, "%.1fx%.1fÏÏ", header.shXSize * dXScale, header.shYSize * dYScale);
// 	memset(header.szImgSize, 0x20, sizeof(header.szImgSize));
// 	memcpy(header.szImgSize, szStrBuf, strlen(szStrBuf));
// 
// 	header.shXSize = MAKE_LE(header.shXSize);
// 	header.shYSize = MAKE_LE(header.shYSize);
// 
// 	sprintf(szStrBuf, "%s.pix", szInImgFile);
// 
// 	FILE *fOut = NULL;
// 	fOut = fopen(szStrBuf, "wb");
// 	if (nullptr == fOut) return;
// 
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	ushort shY = 0;
// 	ushort shX = 0;
// 
// 	bool bFirstInLine = true;
// 	bool bReverse = false;
// 	bool bDot = false;
// 
// 	size_t nLastDotPos = 0;
// 
// 	std::vector<ushort> arrData;
// 	int const nOff = 10;
// 	
// 	for (int j=0; j<srcImg.rows; j++)
// 	{
// 		for (int i = bReverse ? (srcImg.cols-1) : 0; bReverse ? i >= 0 : i<srcImg.cols; bReverse ? --i : ++i)
// 		{
// 			intensity = getLevel(srcImg.at<uchar>(j, i));
// 			
// 			if (intensity == 7)
// 			{
// 				bDot = true;
// 
// 				if (bFirstInLine)
// 				{
// 					shY = 0x8000 | (0x0FFF & j);
// 					shY = MAKE_LE(shY);
// 
// 					arrData.push_back(shY);
// 					bFirstInLine = false;
// 				}
// 
// 				shX = (0x7000 & (intensity << 12)) | (0x0FFF & i);
// 				shX = MAKE_LE(shX);
// 
// 				arrData.push_back(shX);
// 				//nLastDotPos = arrData.size();
// 			}
// 			else if (intensity == 0) // Write empty dot
// 			{
// 				shX = 0x0FFF & i;
// 				shX = MAKE_LE(shX);
// 
// 				if (bFirstInLine)
// 				{
// 					shY = 0x8000 | (0x0FFF & j);
// 					shY = MAKE_LE(shY);
// 
// 					arrData.push_back(shY);
// 					bFirstInLine = false;
// 				}
// 
// 				arrData.push_back(shX);
// 			}
// 		}
// 
// 		if (arrData.size())
// 		{
// 			header.uiDotsCount += arrData.size();
// 
// 			iFileSize += fwrite(&arrData[0], 1, arrData.size()*sizeof(ushort), fOut);
// 			arrData.clear();
// 		}
// 
// 		if(!bFirstInLine)
// 			bReverse = !bReverse;
// 
// 		bFirstInLine = true;
// 		bDot = false;
// 	}
// 
// 	for (int i = BLOCK_SIZE - (iFileSize % BLOCK_SIZE); i > 0; --i)
// 			iFileSize += fwrite(&chPadding, 1, sizeof(char), fOut);
// 
// 	// Update dots count
// 	header.uiDotsCount = MAKE_DWORD_LE(header.uiDotsCount);
// 	fseek(fOut, SEEK_SET, 0);
// 	fwrite(&header, sizeof(header), 1, fOut);
// 
// 	fclose(fOut);
// 
// 	CreateBitmap(szStrBuf);
// }

bool CGattooImg::doHalfTone()
{
	return true;
}

CSize CGattooImg::getImgSize() const
{
	return CSize(m_Img.cols, m_Img.rows);
}

void const * CGattooImg::getImgData() const
{
	return m_Img.data;
}

bool CGattooImg::Load(LPCSTR lpszFilePath)
{
	m_Img = cv::imread(lpszFilePath);
	return !m_Img.empty();
}

bool CGattooImg::Process()
{
	std::vector<IImgFilter*> filters;

	filters.push_back(new CEqulizeFilter());
	filters.push_back(new CHalfToneFilter());

	for(int i=0; i<filters.size(); ++i)
	{
		filters[i]->Apply(m_Img);
	}

	CImgConverter converter;

	char buffer [L_tmpnam];
	tmpnam (buffer);

	cv::imshow("Test", m_Img);

	cv::cvtColor(m_Img, m_Img, CV_RGB2GRAY);

 	converter.Convert(m_Img, buffer);
 	converter.CreateBitmap(buffer);

	cv::cvtColor(m_Img, m_Img, CV_GRAY2RGB);

	return true;
}
