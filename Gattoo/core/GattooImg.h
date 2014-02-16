#pragma once

#include <opencv2/opencv.hpp>

class CGattooImg
{
public:

	CGattooImg(void);
	~CGattooImg(void);

// 	static void CreateBitmap(char const * const szInImgFile);
// 	static void ConvertBitmap(char const * const szInImgFile);
// 	static void ConvertBitmapWithMaxMin(char const * const szInImgFile);
// 	static void ConvertBitmapByColor(char const * const szInImgFile);

	CSize getImgSize() const;
	void const * getImgData() const;

	bool Load(LPCSTR lpszFilePath);
	bool Process();

protected:

	static unsigned char getColor(unsigned char chLevel);
	static char getLevel(uchar intencity);
	//static void getSideDots(cv::Mat &img, int &iMinX, int &iMaxX, int &iMinY, int &iMaxY);
	bool doHalfTone();

	cv::Mat m_Img;
};

