#include "Vehicle.h"

#include "Mod.h"
#include "LightGroups.h"

Vehicle::Vehicle(CVehicle* vehicle)
{
	m_Vehicle = vehicle;
}

void Vehicle::Update()
{
	CheckForLightGroups();
}

void Vehicle::CheckForLightGroups()
{
	auto lightGroups = LightGroups::GetLightGroups(m_Vehicle->m_nModelIndex);

	for (auto lightGroup : lightGroups)
	{
		if (m_LightGroupData.find(lightGroup) != m_LightGroupData.end()) continue;

		auto lightGroupData = new LightGroupData(lightGroup);
		m_LightGroupData[lightGroup] = lightGroupData;
	}
}

void Vehicle::Draw()
{
	char buffer[256];

	CVector vehPosition = m_Vehicle->GetPosition();
	RwV3d rwp = { vehPosition.x, vehPosition.y, vehPosition.z };
	RwV3d screenCoord; float w, h;

	CSprite::CalcScreenCoors(rwp, &screenCoord, &w, &h, true, true);

	CVector2D screenPos({ screenCoord.x, screenCoord.y });

	//

	for (auto pair : m_LightGroupData)
	{
		auto lightGroup = pair.first;
		auto lightGroupData = pair.second;

		sprintf(buffer, "[data] id= %d, points=%d", lightGroup->modelId, lightGroup->points.size());
		DrawScreenText(buffer, screenPos);

		screenPos.y += 20;
	}

	//

	if (m_DrawPoints)
	{
		DrawPoints();
	}
}

void Vehicle::DrawPoints()
{
	char buffer[256];

	for (auto pair : m_LightGroupData)
	{
		auto lightGroup = pair.first;
		//auto lightGroupData = pair.second;

		int i = 0;
		for (auto point : lightGroup->points)
		{
			auto position = lightGroup->GetPointPosition(point, m_Vehicle);
			sprintf(buffer, "[%d]", i + 1);
			DrawWorldText(buffer, position);

			i++;
		}
	}
}

void Vehicle::RegisterCoronas()
{
	int lightId = reinterpret_cast<unsigned int>(m_Vehicle) + 50;

	if (m_DrawPoints)
	{
		for (auto pair : m_LightGroupData)
		{
			auto lightGroup = pair.first;
			//auto lightGroupData = pair.second;

			for (auto point : lightGroup->points)
			{
				auto position = lightGroup->GetPointPosition(point, m_Vehicle);

				CCoronas::RegisterCorona(
					lightId++,
					NULL,
					255,
					0,
					0,
					255,
					position,
					0.2f,
					800.0f,
					eCoronaType::CORONATYPE_SHINYSTAR,
					eCoronaFlareType::FLARETYPE_NONE,
					false,
					false,
					0,
					0.0f,
					false,
					0.01f,
					0,
					100.0f,
					false,
					false
				);
			}
		}
	}
}

void Vehicle::Destroy()
{
	
};
