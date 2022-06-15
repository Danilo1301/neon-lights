#pragma once

#include "pch.h"
#include "VehicleDummy.h"

class Dummy {
public:
	std::string name = "";
	CVector offset = CVector(0, 0, 0);

	CVector GetTransformedPosition(CVehicle* vehicle) {
		if (name.length() == 0) return VehicleDummy::GetTransformedPosition(vehicle, offset);

		return VehicleDummy::FindTransformedDummyPosition(vehicle, name, CVector(0, 0, 0));
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