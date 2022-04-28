#include "NeonLights.h"
#include "VehicleInfoManager.h"
#include "LightGroup.h"
#include "Pattern.h"
#include "KeyPress.h"

bool firstLoad = true;

bool NeonLights::m_EnableDebug = false;

NeonLights::NeonLights() {
	Log::Open();

	Log::file << "Initialized" << std::endl;

	LoadConfig();

	/*
	auto pattern1 = Patterns::CreatePattern("pattern1");
	pattern1->AddColor({ 255, 0, 0 }, 500);
	pattern1->AddColor({ 0, 255, 0 }, 500);
	pattern1->AddColor({ 0, 0, 255 }, 500);

	auto lightGroup1 = LightGroups::CreateLightGroup("group1", 521, pattern1);
	lightGroup1->AddDummy(CVector(-2, 0, 3));
	lightGroup1->AddDummy(CVector(2, 0, 3));
	lightGroup1->AddDummy(CVector(2, 3, 3));
	lightGroup1->AddDummy(CVector(4, 3, 3));
	lightGroup1->amount = 20;

	auto pattern2 = Patterns::CreatePattern("pattern2");
	pattern2->AddColor({ 255, 255, 0 }, 500);
	pattern2->AddColor({ 0, 255, 255 }, 500);
	pattern2->AddColor({ 255, 0, 255 }, 500);

	auto lightGroup2 = LightGroups::CreateLightGroup("group2", 521, pattern2);
	lightGroup2->AddDummy(CVector(-2, 0, 1));
	lightGroup2->AddDummy(CVector(2, 0, 1));
	lightGroup2->amount = 20;
	*/


	Events::vehicleSetModelEvent += [](CVehicle* vehicle, int modelIndex) {
		VehicleInfoManager::TryAddVehicleInfo(vehicle, modelIndex);
	};

	Events::vehicleDtorEvent.after += [](CVehicle* vehicle) {
		if (VehicleInfoManager::HasVehicleInfo(vehicle)) {
			VehicleInfoManager::RemoveVehicleInfo(vehicle);
		}
	};


	Events::drawingEvent += []() {
		VehicleInfoManager::Draw();
	};

	Events::processScriptsEvent += []() {
		VehicleInfoManager::Update();

		KeyPress::Update();

		if (KeyPress::GetKey(17)) {
			if (KeyPress::GetKeyDown(78)) {
				auto veh = FindPlayerVehicle(0, false);

				if (veh > 0) {
					if (VehicleInfoManager::HasVehicleInfo(veh)) {
						auto vehicleInfo = VehicleInfoManager::GetVehicleInfo(veh);
						vehicleInfo->m_Enabled = !vehicleInfo->m_Enabled;
					}
				}
			}
		}

		if (KeyPress::GetKey(17)) {
			if (KeyPress::GetKeyDown(82)) {
				CMessages::AddMessageJumpQ("Config reloaded", 1000, 0, false);

				VehicleInfoManager::RemoveAllVehicleInfos();
				LightGroups::RemoveAllLightGroups();
				Patterns::RemovePatterns();

				LoadConfig();

				for (auto vehicle : CPools::ms_pVehiclePool) {
					VehicleInfoManager::TryAddVehicleInfo(vehicle, vehicle->m_nModelIndex);
				}
			}
		}

		if (KeyPress::GetKey(17) && KeyPress::GetKey(16)) {
			if (KeyPress::GetKeyDown(82)) {
				NeonLights::m_EnableDebug = !NeonLights::m_EnableDebug;
				CMessages::AddMessageJumpQ(NeonLights::m_EnableDebug ? "Debug enabled" : "Debug disabled", 1000, 0, false);
			}
		}
	};
}

void NeonLights::LoadConfig() {
	std::string path = PLUGIN_PATH("\\NeonLights.config.json");
	std::ifstream file(path);

	Log::file << "------------------------------------" << std::endl;
	Log::file << "Loading config from: " << path << std::endl;

	Json::Value cfg;
	Json::Reader reader;

	if (!reader.parse(file, cfg, true)) {
		ShowErrorMessage("Failed to parse configuration", reader.getFormattedErrorMessages());
		return;
	}

	for (auto patternName : cfg["patterns"].getMemberNames())
	{
		auto patternRainbow = Patterns::CreatePattern(patternName);
		auto stepArr = cfg["patterns"][patternName];
		for (size_t i = 0; i < stepArr.size(); i++)
		{
			auto step = stepArr[i];
			auto duration = step["time"].asInt();
			CRGBA color(
				(unsigned char)step["color"][0].asInt(),
				(unsigned char)step["color"][1].asInt(),
				(unsigned char)step["color"][2].asInt(),
				(unsigned char)step["color"][3].asInt()
			);

			patternRainbow->AddColor(color, duration);
		}

		Log::file << "Loaded pattern '" << patternRainbow->name << "' with " << patternRainbow->steps.size() << " steps" << std::endl;
	}

	for (auto groupName : cfg["groups"].getMemberNames())
	{
		auto group = cfg["groups"][groupName];
		auto vehicle = group["vehicle"].asInt();
		auto pattern = Patterns::GetPattern(group["pattern"].asString());

		if (pattern == nullptr) {
			ShowErrorMessage("Error loading group " + groupName, "Pattern not found");
			continue;
		}

		auto lightGroup = LightGroups::CreateLightGroup(groupName, vehicle, pattern);
		lightGroup->amount = group["amount"].asInt();
		lightGroup->offsetBy = group["offsetBy"].asInt();
		lightGroup->size = group["size"].asFloat();
		lightGroup->lerpColor = group["lerpColor"].asBool();
		lightGroup->farClip = ValidateValue(group["farClip"], lightGroup->farClip).asFloat();
		lightGroup->nearClip = ValidateValue(group["nearClip"], lightGroup->nearClip).asFloat();

		for (size_t i = 0; i < group["dummies"].size(); i++)
		{
			Dummy* dummy = Dummy::FromJSON(group["dummies"][i]);
			lightGroup->AddDummy(dummy);
		}

		Log::file << "Loaded group '" << groupName << "' with " << group["dummies"].size() << " dummies for ID " << vehicle << std::endl;

	}

	firstLoad = false;

	Log::file << "------------------------------------" << std::endl;
}

void NeonLights::ShowErrorMessage(std::string title, std::string content) {
	static char buffer[256];
	snprintf(buffer, 256, "%s", content.c_str());

	Log::file << "------------------------------------" << std::endl;
	Log::file << title << std::endl;
	Log::file << content << std::endl;
	Log::file << "------------------------------------" << std::endl;

	if (firstLoad) {
		MessageBox(HWND_DESKTOP, content.c_str(), title.c_str(), MB_ICONERROR);
	}
	else {
		static char buffer[256];
		sprintf(buffer, "%s~n~Check NeonLights.log for more info", title.c_str());
		CMessages::AddMessageJumpQ(buffer, 3000, 0, true);
	}
}

template<class T>
inline Json::Value NeonLights::ValidateValue(Json::Value value, T defaultValue)
{
	if (value.empty()) return defaultValue;
	return value;
}

NeonLights mod;
