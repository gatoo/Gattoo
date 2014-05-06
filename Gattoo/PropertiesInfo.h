#pragma once

#include <map>
#include <set>
#include <string>

class CPropertiesInfo
{
public:
	
	CPropertiesInfo(void);
	virtual ~CPropertiesInfo(void);

	bool SetValue(int propType, _variant_t const &value);
	void GetAllProperties(CMFCPropertyGridCtrl &grid);

protected:

	virtual void FillDefaultValues() = 0;
	virtual LPCTSTR getPropName(int enProp) = 0;
	virtual LPCTSTR getGroupName(int enProp) = 0;

	typedef std::set<CMFCPropertyGridProperty*> CGroupsSet;
	typedef std::map<int, CMFCPropertyGridProperty*> CPropertiesMap;

	CGroupsSet m_setGroups;
	CPropertiesMap m_mapProps;

	void ClearProps();
	CMFCPropertyGridProperty* AddValue(int propType, CMFCPropertyGridProperty* pGroup = nullptr, _variant_t &value = _variant_t(), LPCTSTR lpszDescription = nullptr);
};

