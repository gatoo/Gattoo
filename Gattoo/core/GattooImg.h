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

	enum EImageState
	{
		enUnknown,
		enInitial,
		enHalftone,
		enCompleted
	};

	CSize getImgSize() const;
	std::string const & getImagePath() const;
	char getImageDepth() const;

	void const * getImgData() const;

	bool Load(LPCSTR lpszFilePath);
	bool doHalfTone();
	bool saveToSD();

	void Draw(CDC* pDC, CRect const &rc);

	//bool IsLoaded();

	EImageState getState() const;

protected:

	static unsigned char getColor(unsigned char chLevel);
	static char getLevel(uchar intencity);
	//static void getSideDots(cv::Mat &img, int &iMinX, int &iMaxX, int &iMinY, int &iMaxY);
	
	bool getDriveToSave(std::basic_string<TCHAR> &strDrive);
	bool m_bIsChanged;

	int m_ZoomFactor;

	EImageState m_enState;

	std::string m_strImgPath;
	
	BYTE* m_BMPBuff;
	cv::Mat m_Img;
	CDC m_memDC;
};

