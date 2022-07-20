#pragma once

#include "pch.h"

struct CoronaGroup
{
	CVector startOffset;
	std::vector<CVector> offsets;
	float totalDistance = 0.0f;
	float jumpDistance = 0.0f;
	bool invertOffset = false;
	bool keepGlobalOffset = false;

	void UpdateTotalDistance() {
		totalDistance = TotalDistanceBetweenPoints(offsets);
	}
};