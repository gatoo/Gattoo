#include "StdAfx.h"
#include "ImgPropertiesInfo.h"


CImgPropertiesInfo::CImgPropertiesInfo(void)
{
	FillDefaultValues();
}


CImgPropertiesInfo::~CImgPropertiesInfo(void)
{
}

void CImgPropertiesInfo::FillDefaultValues()
{
	ClearProps();

	CMFCPropertyGridProperty* pGroup = AddValue(enOIPath);

	AddValue(enOIPath, pGroup, (_variant_t) _T(""), _T("File location."));
	AddValue(enOIDepth, pGroup, (_variant_t) _T(""), _T("Bits per pixel."));
	AddValue(enOISize, pGroup, (_variant_t) _T(""), _T("Specifies the original image size, px."));
	AddValue(enOIModification, pGroup, (_variant_t) _T(""), _T("File modification time."));

	pGroup = AddValue(enPISize);

	AddValue(enPISize, pGroup, (_variant_t) _T(""), _T("Specifies the printing image size, mm."));
}

LPCTSTR CImgPropertiesInfo::getGroupName(int enProp)
{
	LPCTSTR lpszResult = nullptr;

	switch(enProp)
	{
	case enOIPath:
	case enOIDepth:
	case enOISize:
	case enOIModification:
		lpszResult = _T("Original image");
		break;
	case enPISize:
		lpszResult = _T("Image to print");
		break;
	}

	assert(lpszResult);
	return lpszResult;
}

LPCTSTR CImgPropertiesInfo::getPropName(int enProp)
{
	LPCTSTR lpszResult = nullptr;

	switch(enProp)
	{
	case enOIPath:
		lpszResult = _T("Path");
		break;
	case enOIDepth:
		lpszResult = _T("Depth");
		break;
	case enOISize:
		lpszResult = _T("Size");
		break;
	case enOIModification:
		lpszResult = _T("Date");
		break;
	case enPISize:
		lpszResult = _T("Size");
		break;
	}

	assert(lpszResult);
	return lpszResult;
}

