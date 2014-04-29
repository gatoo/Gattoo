#pragma once

#include <map>
#include <string>

class CPropertiesInfo
{
public:
	
	CPropertiesInfo(void);
	virtual ~CPropertiesInfo(void);

// 	struct SPropInfo
// 	{
// 		_variant_t value;
// 		std::string strDesc;
// 
// 		SPropInfo() {}
// 		SPropInfo(_variant_t val, LPCTSTR lpszDesc) : value(val), strDesc(lpszDesc) {}
// 	};

	typedef std::map<std::string, CMFCPropertyGridProperty*> CPropsMap;
	typedef std::map<std::string, CPropsMap> CGroupsMap;

	bool SetValue(LPCTSTR lpszGroup, LPCTSTR lpszName, _variant_t &value, LPCTSTR lpszDescription);

	void GetAllProperties(CMFCPropertyGridCtrl &grid) const;

protected:

	virtual void FillDefaultValues() = 0;

	CGroupsMap m_Props;
};

