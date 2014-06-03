#include "stdafx.h"
#include "GlobalSettings.h"

// TODO: Check scale values for hz and vt axis
double const CStaticSettings::HZ_SIZE_SCALE = 0.25;
double const CStaticSettings::VT_SIZE_SCALE = 0.25;

// TODO: Check maximum values for image width and height in px and mm
unsigned int  const CStaticSettings::HZ_SIZE_MAX_VALUE = 1024;
unsigned int  const CStaticSettings::VT_SIZE_MAX_VALUE = 2048;

int const CStaticSettings::HZ_SCROLL_STEP = 15;
int const CStaticSettings::VT_SCROLL_STEP = CStaticSettings::HZ_SCROLL_STEP;
