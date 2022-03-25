#pragma once

#include "pch.h"
#include "json/json.h"

class Dummy {
public:
	std::string name;
	CVector position;

	static Dummy* FromJSON(Json::Value value) {
		Dummy* dummy = new Dummy();

		if (value.isArray()) {
			dummy->position.x = value[0].asFloat();
			dummy->position.y = value[1].asFloat();
			dummy->position.z = value[2].asFloat();
		}
		else {
			dummy->name = value.asString();
		}

		return dummy;
	}
};