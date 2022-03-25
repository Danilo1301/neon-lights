#pragma once

#include "pch.h"

class VehicleDummies {
public:
	static std::vector<RwFrame*> GetFramesOnVehicle(CVehicle* vehicle);
	static CVector GetDummyPosition(RwFrame* frame);
private:
	static void FindDummies(CVehicle* vehicle, RwFrame* frame, bool parent = false);
};