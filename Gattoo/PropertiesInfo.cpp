#include "StdAfx.h"
#include "PropertiesInfo.h"


CPropertiesInfo::CPropertiesInfo(void)
{
}

CPropertiesInfo::~CPropertiesInfo(void)
{
	ClearProps();
}

void CPropertiesInfo::GetAllProperties(CMFCPropertyGridCtrl &grid)
{
	CGroupsSet::iterator iter = m_setGroups.begin();

	while(iter != m_setGroups.end())
	{
		grid.AddProperty(*iter);
		iter = m_setGroups.erase(iter);
	}
}

CMFCPropertyGridProperty* CPropertiesInfo::AddValue(int propType, CMFCPropertyGridProperty* pGroup /*= nullptr*/, _variant_t &value /*= _variant_t()*/, LPCTSTR lpszDescription /*= nullptr*/)
{
	if (pGroup == nullptr)
	{
		// Create group
		pGroup = *m_setGroups.insert(new CMFCPropertyGridProperty(getGroupName(propType))).first;
	}
	else
	{
		// Create property
		CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(getPropName(propType), value, lpszDescription);
		m_mapProps[propType] = pProp;
		
		pProp->AllowEdit(FALSE);

		pGroup->AddSubItem(pProp);
		pGroup = nullptr;
	}

	return pGroup;
}

bool CPropertiesInfo::SetValue(int propType, _variant_t const &value)
{
	CPropertiesMap::iterator iter = m_mapProps.find(propType);

	if (m_mapProps.end() == iter)
	{
		LogErr(_T("Failed to find property with id=%d"), propType);
		return false;
	}

	iter->second->SetValue(value);
	return true;
}

void CPropertiesInfo::ClearProps()
{
	CGroupsSet::iterator iter = m_setGroups.begin();

	for(iter; iter != m_setGroups.end(); ++iter)
		delete (*iter);

	m_setGroups.clear();
}
