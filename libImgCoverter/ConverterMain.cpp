#include "stdafx.h"
#include "Interfaces/IIMgConverter.h"
#include "ImgConverters/ImgConverter.h"

IImgConverter* createConverter(EImgConverter enType)
{
	IImgConverter* pConverter = nullptr;

	switch(enType)
	{
	case enDefault:
		pConverter = new CImgConverter;
		break;
	}

	return pConverter;
}