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
	bool doHalfTone();
	bool saveToSD();

	void Draw(CDC* pDC, CRect const &rc);

protected:

	static unsigned char getColor(unsigned char chLevel);
	static char getLevel(uchar intencity);
	//static void getSideDots(cv::Mat &img, int &iMinX, int &iMaxX, int &iMinY, int &iMaxY);
	
	bool getDriveToSave(std::basic_string<TCHAR> &strDrive);
	bool m_bIsChanged;
	
	BYTE* m_BMPBuff;
	cv::Mat m_Img;
	CDC m_memDC;
};

