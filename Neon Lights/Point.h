#pragma once

#include "pch.h"
#include "VehicleDummy.h"

class Point {
public:
	CVector offset = CVector(0, 0, 0);

	CVector GetOffset()
	{
		return offset;
	}
};