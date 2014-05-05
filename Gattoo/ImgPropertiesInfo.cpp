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
	m_Props.clear();

	std::pair<CGroupsMap::iterator,bool> group;
	std::pair<LPCTSTR, CMFCPropertyGridProperty*> valueData;

	group = m_Props.insert(CGroupsMap::value_type(_T("Original Image"), CPropsMap()));
	group.first->second.insert(CPropsMap::value_type(_T("Path"), new CMFCPropertyGridProperty(_T("Path"), (_variant_t) _T(""), _T("File location."))));
	group.first->second.insert(CPropsMap::value_type(_T("Date"), new CMFCPropertyGridProperty(_T("Date"), (_variant_t) _T(""), _T("File modification time."))));
	group.first->second.insert(CPropsMap::value_type(_T("Depth"), new CMFCPropertyGridProperty(_T("Depth"), (_variant_t) _T(""), _T("Bits per pixel."))));
	group.first->second.insert(CPropsMap::value_type(_T("Size"), new CMFCPropertyGridProperty(_T("Size"), (_variant_t) _T(""), _T("Specifies the original image size, px."))));


	/***********/

	group = m_Props.insert(CGroupsMap::value_type(_T("Image to print"), CPropsMap()));
	group.first->second.insert(CPropsMap::value_type(_T("Size"), new CMFCPropertyGridProperty(_T("Size"), (_variant_t) _T(""), _T("Specifies the printing image size, mm."))));
}

bool CImgPropertiesInfo::GetPropInfoByID(int propType, LPCTSTR* lpszGroup, LPCTSTR* lpszName, LPCTSTR* lpszDescription)
{
	throw std::exception("The method or operation is not implemented.");
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

