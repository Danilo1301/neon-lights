#pragma once

#include "pch.h"
#include "VehicleInfo.h"

class VehicleInfoManager {
	static std::map<CVehicle*, VehicleInfo*> m_VehicleInfoMap;
public:
	static void Update();
	static void Draw();

	static void TryAddVehicleInfo(CVehicle* vehicle, int modelId);
	static bool HasVehicleInfo(CVehicle* vehicle);
	static VehicleInfo* AddVehicleInfo(CVehicle* vehicle, int modelId);
	static VehicleInfo* GetVehicleInfo(CVehicle* vehicle);
	static void RemoveVehicleInfo(CVehicle* vehicle);
	static void RemoveAllVehicleInfos();
};