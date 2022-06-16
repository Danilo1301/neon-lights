#include "LightGroups.h"

std::map<int, std::vector<LightGroup*>> LightGroups::m_Groups;

LightGroup* LightGroups::CreateLightGroup(int modelId)
{
	LightGroup* lightGroup = new LightGroup(modelId);
	m_Groups[modelId].push_back(lightGroup);
	return lightGroup;
}

std::vector<LightGroup*> LightGroups::GetLightGroups()
{
	std::vector<LightGroup*> groups;
	for (auto p : m_Groups)	for (auto v : p.second) groups.push_back(v);
	return groups;
}

std::vector<LightGroup*> LightGroups::GetLightGroups(int modelId)
{
	std::vector<LightGroup*> groups;
	for (auto p : m_Groups)
	{
		for (auto v : p.second)
		{
			if (v->modelId != modelId) continue;
			groups.push_back(v);
		}
	}
	return groups;
}