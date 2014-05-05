#pragma once
#include "propertiesinfo.h"

class CImgPropertiesInfo : 	public CPropertiesInfo
{
public:
	CImgPropertiesInfo(void);
	virtual ~CImgPropertiesInfo(void);

	enum EProperty
	{
		enOIPath,
		enOIDepth,
		enOISize,
		enOIModification,
		enPISize
	};

	//bool SetValue(EProperty propType, _variant_t &value);

protected:

	virtual LPCTSTR getGroupName(int enProp);
	virtual LPCTSTR getPropName(int enProp);

	virtual void FillDefaultValues();
	virtual bool GetPropInfoByID(int propType, LPCTSTR* lpszGroup, LPCTSTR* lpszName, LPCTSTR* lpszDescription);

};

