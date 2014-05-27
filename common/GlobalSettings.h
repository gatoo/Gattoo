#pragma once

class CStaticSettings
{
public:

	// Horizontal and vertical axises scale for px to mm conversion
	static double const HZ_SIZE_SCALE;
	static double const VT_SIZE_SCALE;

	// Maximum supported horizontal and vertical dots count
	static unsigned int const HZ_SIZE_MAX_VALUE;
	static unsigned int const VT_SIZE_MAX_VALUE;
};
