#include "StdAfx.h"
#include "PropertiesInfo.h"


CPropertiesInfo::CPropertiesInfo(void)
{
}

CPropertiesInfo::~CPropertiesInfo(void)
{
}

void CPropertiesInfo::GetAllProperties(CMFCPropertyGridCtrl &grid) const
{
	CMFCPropertyGridProperty* pProp = nullptr;
	CMFCPropertyGridProperty* pGroup = nullptr;

	CGroupsMap::const_iterator iterGroup = m_Props.begin();

	while(iterGroup != m_Props.end())
	{
		pGroup = new CMFCPropertyGridProperty(iterGroup->first.c_str());

		CPropsMap::const_iterator iterProp = iterGroup->second.begin();

		while(iterProp != iterGroup->second.end())
		{
			pProp = iterProp->second;
			pProp->AllowEdit(FALSE);
			pGroup->AddSubItem(pProp);

			++iterProp;
		}

		grid.AddProperty(pGroup);

		++iterGroup;
	}
}

bool CPropertiesInfo::SetValue(LPCTSTR lpszGroup, LPCTSTR lpszName, _variant_t &value, LPCTSTR lpszDescription)
{
	CGroupsMap::iterator iterGroup = m_Props.find(lpszGroup);

	if (iterGroup == m_Props.end())
	{
		CPropsMap tmpProp;

		tmpProp[lpszName] = new CMFCPropertyGridProperty(lpszName, value, lpszDescription);
		m_Props[lpszGroup] = tmpProp;
	}
	else
	{
		CPropsMap::iterator iterProp = iterGroup->second.find(lpszName);

		if (iterProp == iterGroup->second.end())
		{
			iterGroup->second.insert(CPropsMap::value_type(lpszName, new CMFCPropertyGridProperty(lpszName, value, lpszDescription)));
		}
		else
		{
			iterProp->second->SetValue(value);
			iterProp->second->SetDescription(lpszDescription);
		}
		
	}

	return true;
}
