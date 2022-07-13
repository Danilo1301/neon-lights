#pragma once

#include "pch.h"
#include "VehicleDummy.h"

class Dummy {
public:
	std::string name = "";
	CVector offset = CVector(0, 0, 0);

	CVector GetPosition(CVehicle* vehicle)
	{
		return VehicleDummy::GetTransformedPosition(vehicle, GetOffset());
	}

	CVector GetOffset()
	{
		return offset;
	}

	void UpdateOffset(CVehicle* vehicle)
	{
		if (name.empty()) return;

		offset = VehicleDummy::GetDummyOffset(vehicle, name);
	}

	static Dummy* FromJSON(Json::Value value) {
		Dummy* dummy = new Dummy();

		if (value.isArray()) {
			dummy->offset.x = value[0].asFloat();
			dummy->offset.y = value[1].asFloat();
			dummy->offset.z = value[2].asFloat();
		}
		else {
			dummy->name = value.asString();
		}

		return dummy;
	}
	
	static Json::Value ToJSON() {
		Json::Value value = Json::objectValue;

		return value;
	}

};