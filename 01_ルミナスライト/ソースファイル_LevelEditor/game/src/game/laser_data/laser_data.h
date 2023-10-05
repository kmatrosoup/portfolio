#pragma once

#include "../direction_id/direction_id.h"
#include "../color_id/color_id.h"

struct SLaserData
{
	SLaserData(DIRECTION_ID direction, COLOR_ID color)
		: direction(direction)
		, color(color)
	{}

	DIRECTION_ID direction;	// ï˚å¸ID
	COLOR_ID color;			// êFID
};
