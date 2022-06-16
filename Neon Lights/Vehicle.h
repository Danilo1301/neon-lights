#pragma once

#include "pch.h"
#include "LightGroup.h"

struct LightGroupData
{
	LightGroup* lightGroup;

	LightGroupData(LightGroup* lightGroup)
	{
		this->lightGroup = lightGroup;
	}
};

class Vehicle {
private:
	std::map<LightGroup*, LightGroupData*> m_LightGroupData;
public:
	CVehicle* m_Vehicle;

	bool m_DrawPoints = true;

	Vehicle(CVehicle* vehicle);

	void Update();
	void CheckForLightGroups();

	void Draw();
	void DrawPoints();

	void RegisterCoronas();

	void Destroy();

};