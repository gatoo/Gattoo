#pragma once

#include <map>
#include <string>

class CPropertiesInfo
{
public:
	
	CPropertiesInfo(void);
	virtual ~CPropertiesInfo(void);

	typedef std::map<std::string, CMFCPropertyGridProperty*> CPropsMap;
	typedef std::map<std::string, CPropsMap> CGroupsMap;

	

	//bool SetValue(int propType, _variant_t &value);
	void GetAllProperties(CMFCPropertyGridCtrl &grid) const;

protected:

	virtual LPCTSTR getGroupName(int enProp) = 0;
	virtual LPCTSTR getPropName(int enProp) = 0;

	virtual bool GetPropInfoByID(int propType, LPCTSTR* lpszGroup, LPCTSTR* lpszName, LPCTSTR* lpszDescription) = 0;
	bool SetValue(LPCTSTR lpszGroup, LPCTSTR lpszName, _variant_t &value, LPCTSTR lpszDescription);

	virtual void FillDefaultValues() = 0;

	CGroupsMap m_Props;
};

