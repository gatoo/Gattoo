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
