#pragma once
#include "propertiesinfo.h"

class CImgPropertiesInfo : 	public CPropertiesInfo
{
public:
	CImgPropertiesInfo(void);
	virtual ~CImgPropertiesInfo(void);

protected:

	virtual void FillDefaultValues();
};

