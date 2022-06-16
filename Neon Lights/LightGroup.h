#pragma once

#include "pch.h"
#include "Point.h"

class LightGroup {
public:
	int modelId;
	std::string name = "Light Group";

	CVector position = CVector(0, 0, 0);

	int amountLights = 30;

	std::vector<Point*> points;

	LightGroup(int modelId);

	Point* AddPoint(CVector offset);

	CVector GetPointPosition(Point* point, CVehicle* vehicle);
};