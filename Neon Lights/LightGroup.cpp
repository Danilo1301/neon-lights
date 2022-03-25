#include "LightGroup.h"

std::map<int, std::vector<LightGroup*>> LightGroups::m_LightGroups;

LightGroup* LightGroups::CreateLightGroup(std::string name, int modelId, Pattern* pattern) {
	LightGroup* lightGroup = new LightGroup();
	lightGroup->name = name;
	lightGroup->modelId = modelId;
	lightGroup->pattern = pattern;
	m_LightGroups[modelId].push_back(lightGroup);
	return lightGroup;
}

void LightGroups::RemoveAllLightGroups() {
	for (auto p : m_LightGroups) {
		for (auto lightGroup : p.second) {
			delete lightGroup;
		}
		p.second.clear();
	}
	m_LightGroups.clear();
}

bool LightGroups::HasLightGroupForModel(int modelId) {
	for (auto p : m_LightGroups) {
		for (auto lightGroup : p.second) {
			if (lightGroup->modelId == modelId) return true;
		}
	}
	return false;
}