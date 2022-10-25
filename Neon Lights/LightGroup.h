#pragma once

#include "pch.h"
#include "Dummy.h"
#include "Pattern.h"

struct LightGroupCloneSettings
{
	Dummy* toDummy;
	bool flipX;
	bool flipY;
	bool invertOffset;
	CVector offset = CVector(0, 0, 0);
};

class LightGroup {
public:
	std::string name;
	int modelId;
	Pattern* pattern;
	std::vector<Dummy*> dummies;
	std::vector<LightGroupCloneSettings*> clones;

	float lightsDistance = 0.1f;
	float size = 0.2f;
	int offsetBy = 0;
	bool lerpColor = true;
	float farClip = 600.0f;
	float nearClip = 0.05f;
	bool fixPatternOffset = false;

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