#include "Mod.h"
#include "Vehicles.h"
#include "LightGroup.h"
#include "Pattern.h"

bool firstLoad = true;

bool Mod::m_EnableDebug = false;

std::string Mod::m_Version = "1.3.1";
std::string Mod::m_Name = "Neon Lights";

Mod::Mod() {
	Log::Open("NeonLights.log");

	Log::file << "Initialized v" << m_Version << std::endl;

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
		//Vehicles::TryAddVehicleInfo(vehicle, modelIndex);
	};

	Events::vehicleDtorEvent.after += [](CVehicle* vehicle) {
		if (Vehicles::HasVehicle(vehicle)) {
			Vehicles::RemoveVehicle(vehicle);
		}
	};

	Events::drawingEvent +=	Draw;
	Events::processScriptsEvent += Update;
	Events::vehicleRenderEvent += [](CVehicle* veh) {
		if (Vehicles::HasVehicle(veh)) {
			Vehicles::m_VehicleMap[veh]->RegisterCoronas();
		}
	};
}

void Mod::Update() {
	Input::Update();

	Menu::Update();

	if (Input::GetKey(17)) {
		if (Input::GetKeyDown(78)) {
			auto veh = FindPlayerVehicle(0, false);

			if (veh > 0) {
				if (Vehicles::HasVehicle(veh)) {
					auto vehicle = Vehicles::GetVehicle(veh);
					vehicle->m_Enabled = !vehicle->m_Enabled;
				}
			}
		}
	}

	if (Input::GetKey(17)) {
		if (Input::GetKeyDown(82)) {
			CMessages::AddMessageJumpQ("Config reloaded", 1000, 0, false);

			Vehicles::RemoveAllVehicles();
			LightGroups::RemoveAllLightGroups();
			Patterns::RemovePatterns();

			LoadConfig();

			Vehicles::TryAddAllVehicles();
		}
	}

	if (Input::GetKey(17) && Input::GetKey(16)) {
		if (Input::GetKeyDown(68)) {
			Mod::m_EnableDebug = !Mod::m_EnableDebug;
			CMessages::AddMessageJumpQ(Mod::m_EnableDebug ? "Debug enabled" : "Debug disabled", 1000, 0, false);
		}
	}

	Vehicles::TryAddAllVehicles();

	for (auto p : Vehicles::m_VehicleMap) {
		Vehicle* vehicle = p.second;
		vehicle->Update();
	}
}

void Mod::Draw() {

	for (auto p : Vehicles::m_VehicleMap) {
		Vehicle* vehicle = p.second;

		vehicle->Draw();

		if (Mod::m_EnableDebug) {
			vehicle->DrawDebug();
		}
	}
}

void Mod::LoadConfig() {
	std::string path = PLUGIN_PATH("\\NeonLights.config.json");
	std::ifstream file(path);

	Log::file << "------------------------------------" << std::endl;
	Log::file << "* RELOAD *" << std::endl;
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
		//lightGroup->amount = group["amount"].asInt();
		lightGroup->lightsDistance = ValidateValue(group["lights_distance"], lightGroup->lightsDistance).asFloat();
		lightGroup->offsetBy = group["offsetBy"].asInt();
		lightGroup->size = group["size"].asFloat();
		lightGroup->lerpColor = group["lerpColor"].asBool();
		lightGroup->farClip = ValidateValue(group["farClip"], lightGroup->farClip).asFloat();
		lightGroup->nearClip = ValidateValue(group["nearClip"], lightGroup->nearClip).asFloat();
		lightGroup->fixPatternOffset = ValidateValue(group["fixPatternOffset"], lightGroup->fixPatternOffset).asBool();

		for (size_t i = 0; i < group["dummies"].size(); i++)
		{
			Dummy* dummy = Dummy::FromJSON(group["dummies"][i]);
			lightGroup->AddDummy(dummy);
		}

		if (!group["clone_to"].isNull())
		{
			for (size_t i = 0; i < group["clone_to"].size(); i++)
			{
				Json::Value cloneValue = group["clone_to"][i];

				auto clone = new LightGroupCloneSettings({
					Dummy::FromJSON(cloneValue["dummy"]),
					ValidateValue(cloneValue["flipX"], false).asBool(),
					ValidateValue(cloneValue["flipY"], false).asBool(),
					ValidateValue(cloneValue["invertOffset"], false).asBool()
				});
				

				/*
				auto dummy = Dummy::FromJSON(group["clone_to"][i]);

				auto clone = new LightGroupCloneSettings({
					dummy,
					false,
					false
				});
				*/

				lightGroup->clones.push_back(clone);
			}
		}

		Log::file << "Loaded group '" << groupName << "' with " << group["dummies"].size() << " dummies for ID " << vehicle << std::endl;

	}

	firstLoad = false;

	Log::file << "------------------------------------" << std::endl;
}

void Mod::ShowErrorMessage(std::string title, std::string content) {
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
inline Json::Value Mod::ValidateValue(Json::Value value, T defaultValue)
{
	if (value.empty()) return defaultValue;
	return value;
}

Mod mod;
