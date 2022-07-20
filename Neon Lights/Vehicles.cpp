#include "Vehicles.h"

#include "Mod.h"

std::map<CVehicle*, Vehicle*> Vehicles::m_VehicleMap;

void Vehicles::TryAddVehicle(CVehicle* veh) {
	if (HasVehicle(veh)) return;

	auto modelId = veh->m_nModelIndex;

	if (!LightGroups::HasLightGroupForModel(modelId)) return;

	AddVehicle(veh);
}

bool Vehicles::HasVehicle(CVehicle* vehicle) {
	return m_VehicleMap.find(vehicle) != m_VehicleMap.end();
}

Vehicle* Vehicles::AddVehicle(CVehicle* veh) {
	Vehicle* vehicle = new Vehicle(veh);
	m_VehicleMap.insert(std::pair<CVehicle*, Vehicle*>(veh, vehicle));

	Log::file << "[Vehicles] AddVehicle " << veh << ", ID " << veh->m_nModelIndex << std::endl;

	return vehicle;
}

Vehicle* Vehicles::GetVehicle(CVehicle* veh) {
	if (!HasVehicle(veh)) return NULL;
	return m_VehicleMap[veh];
}

void Vehicles::RemoveVehicle(CVehicle* veh) {
	Log::file << "[Vehicles] Removed vehicle info " << veh << ", ID " << veh->m_nModelIndex << std::endl;

	auto it = m_VehicleMap.find(veh);
	auto vehicle = it->second;

	m_VehicleMap.erase(it);

	vehicle->Destroy();
	delete vehicle;
}

void Vehicles::RemoveAllVehicles() {
	std::vector<CVehicle*> vec;
	for (auto p : m_VehicleMap) vec.push_back(p.first);
	for (auto veh : vec) RemoveVehicle(veh);
}

void Vehicles::TryAddAllVehicles() {
	for (auto veh : CPools::ms_pVehiclePool) TryAddVehicle(veh);
}