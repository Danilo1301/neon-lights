#pragma once

#include "pch.h"
#include "LightGroup.h"

struct LightGroupData {
	LightGroup* lightGroup;
	std::vector<CVector> lightStoredPositions;
	int patternProgress;
};

class Vehicle {
public:
	CVehicle* m_Vehicle;

	std::vector<LightGroupData*> m_LightGroupData;
	bool m_Enabled = true;

	Vehicle(CVehicle* vehicle);

	void Update();
	void Draw();
	void DrawDebug();
	void RegisterCoronas();
	void Destroy();

	void CheckForLightGroups();

	void AddLightGroup(LightGroup* lightGroup);
};