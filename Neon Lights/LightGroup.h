#pragma once

#include "pch.h"
#include "Dummy.h"
#include "Pattern.h"

class LightGroup {
public:
	std::string name;
	int modelId;
	Pattern* pattern;
	std::vector<Dummy*> dummies;

	int amount = 1;
	float size = 0.2f;
	int offsetBy = 0;
	bool lerpColor = true;
	float farClip = 600.0f;
	float nearClip = 0.05f;

	Dummy* AddDummy(CVector offset) {
		Dummy* dummy = new Dummy();
		dummy->name = "";
		dummy->offset = offset;
		AddDummy(dummy);
		return dummy;
	}

	void AddDummy(Dummy* dummy) {
		dummies.push_back(dummy);
	}

};

class LightGroups {
public:
	static std::map<int, std::vector<LightGroup*>> m_LightGroups;

	static LightGroup* CreateLightGroup(std::string name, int modelId, Pattern* pattern);
	static void RemoveAllLightGroups();
	static bool HasLightGroupForModel(int modelId);
};