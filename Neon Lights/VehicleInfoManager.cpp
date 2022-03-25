#include "VehicleInfoManager.h"

std::map<CVehicle*, VehicleInfo*> VehicleInfoManager::m_VehicleInfoMap;

void VehicleInfoManager::Update() {
	for (auto p : m_VehicleInfoMap) {
		VehicleInfo* vehicleInfo = p.second;
		vehicleInfo->Update();
		vehicleInfo->RegisterCoronas();
	}
}

void VehicleInfoManager::Draw() {
	for (auto p : m_VehicleInfoMap) {
		VehicleInfo* vehicleInfo = p.second;
		vehicleInfo->Draw();
	}
}

void VehicleInfoManager::TryAddVehicleInfo(CVehicle* vehicle, int modelId) {

	if (HasVehicleInfo(vehicle)) return;

	if (!LightGroups::HasLightGroupForModel(modelId)) return;

	Log::file << "Added vehicle info " << vehicle << ", ID " << modelId << std::endl;

	AddVehicleInfo(vehicle, modelId);
}

bool VehicleInfoManager::HasVehicleInfo(CVehicle* vehicle) {
	return m_VehicleInfoMap.find(vehicle) != m_VehicleInfoMap.end();
}

VehicleInfo* VehicleInfoManager::AddVehicleInfo(CVehicle* vehicle, int modelId) {
	VehicleInfo* vehicleInfo = new VehicleInfo(vehicle, modelId);
	m_VehicleInfoMap.insert(std::pair<CVehicle*, VehicleInfo*>(vehicle, vehicleInfo));
	return vehicleInfo;
}

VehicleInfo* VehicleInfoManager::GetVehicleInfo(CVehicle* vehicle) {
	return m_VehicleInfoMap[vehicle];
}

void VehicleInfoManager::RemoveVehicleInfo(CVehicle* vehicle) {
	auto it = m_VehicleInfoMap.find(vehicle);
	m_VehicleInfoMap.erase(it);

	auto vehicleData = it->second;
	vehicleData->Destroy();
	delete vehicleData;

	Log::file << "Removed vehicle info " << vehicle << ", ID " << vehicle->m_nModelIndex << std::endl;
}

void VehicleInfoManager::RemoveAllVehicleInfos() {
	std::vector<CVehicle*> vec;
	for (auto p : m_VehicleInfoMap) vec.push_back(p.first);
	for (auto veh : vec) RemoveVehicleInfo(veh);
}
