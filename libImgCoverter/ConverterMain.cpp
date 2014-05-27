#include "stdafx.h"
#include "Interfaces/IIMgConverter.h"
#include "ImgConverters/ImgConverter.h"

IImgConverter* createImgConverter(IImgConverter::EImgConverter enType)
{
	IImgConverter* pConverter = nullptr;

	switch(enType)
	{
	case IImgConverter::enDefault:
		pConverter = new CImgConverter;
		break;
	}

	return pConverter;
}