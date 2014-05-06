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

protected:

	virtual void FillDefaultValues();
	virtual LPCTSTR getPropName(int enProp);
	virtual LPCTSTR getGroupName(int enProp);
};

