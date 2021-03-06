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

	// Scroll increment steps
	static int const HZ_SCROLL_STEP;
	static int const VT_SCROLL_STEP;

	// Zoom factor
	static float const IMG_ZOOM_FACTOR_MULTIPLIER;
};
