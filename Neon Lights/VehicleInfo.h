#pragma once

#include "pch.h"
#include "LightGroup.h"

struct LightGroupData {
	LightGroup* lightGroup;
	std::vector<CVector> lightStoredPositions;
	int patternProgress;
};

class VehicleInfo {
public:
	CVehicle* m_Vehicle;

	std::vector<LightGroupData*> m_LightGroupData;
	bool m_Enabled = true;

	VehicleInfo(CVehicle* vehicle, int modelId);

	void Update();
	void Draw();
	void RegisterCoronas();
	void Destroy();

	void AddLightGroup(LightGroup* lightGroup);
};