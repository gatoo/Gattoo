#pragma once

#include <opencv2/opencv.hpp>
#include "../external/progress/UPDialog.h"

class CGattooImg
{
public:

	CGattooImg(void);
	CGattooImg(CGattooImg& src);
	~CGattooImg(void);

	CGattooImg & operator=(const CGattooImg & rhs);

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
	bool doInverse();

	bool IsBinary() const;
	
	bool saveToSD();
	bool saveToFile(LPCTSTR lpszPath);

	void Draw(CBitmap &bmpOut, CPoint const &ptStart);
	void DrawPreview(CDC * pDC);

	bool EraseRect(CRect &rcErase);
	void CropImage(CRect & rc);
	bool Resize(CSize & szNewSize);

	//bool IsLoaded();

	EImageState getState() const;

protected:

	static unsigned char getColor(unsigned char chLevel);
	static char getLevel(uchar intencity);
	//static void getSideDots(cv::Mat &img, int &iMinX, int &iMaxX, int &iMinY, int &iMaxY);
	
	bool getDriveToSave(std::basic_string<TCHAR> &strDrive);
	static bool ThreadProc(const CUPDUPDATA*);

	EImageState m_enState;

	std::string m_strImgPath;
	
	cv::Mat m_Img;
};

