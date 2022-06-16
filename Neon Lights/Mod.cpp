#include "Mod.h"

#include "Vehicles.h"
#include "LightGroups.h"
#include "Patterns.h"
#include "Config.h"
#include "localization/Localization.h"

#include "windows/WindowMain.h"

bool Mod::m_EnableDebug = false;

std::string Mod::m_Version = "2.0.0";
std::string Mod::m_Name = "Neon Lights";
bool Mod::m_IsSamp = false;

//

bool firstLoad = true;

Mod::Mod()
{
	Log::Open("NeonLights.log");
	Log::file << "Initialized v" << m_Version << std::endl;

	if (GetModuleHandle("SAMP.dll"))
	{
		m_IsSamp = true;
		Menu::m_DefaultPosition.y += 140.0f;
	}

	Config::LoadJSON();

	if (Patterns::m_Patterns.size() == 0)
	{
		auto pattern1 = Patterns::CreatePattern("pattern1");
		pattern1->AddColor({ 255, 0, 0 }, 500);
		pattern1->AddColor({ 0, 255, 0 }, 500);
		pattern1->AddColor({ 0, 0, 255 }, 500);

		auto pattern2 = Patterns::CreatePattern("pattern2");
		pattern2->AddColor({ 255, 255, 0 }, 500);
		pattern2->AddColor({ 0, 255, 255 }, 500);
		pattern2->AddColor({ 255, 0, 255 }, 500);
	}

	Config::SaveJSON();

	//

	auto lightGroup1 = LightGroups::CreateLightGroup(481);
	lightGroup1->AddPoint(CVector(-1, 0, 0));
	lightGroup1->AddPoint(CVector(0, 1, 0));
	lightGroup1->AddPoint(CVector(1, 0, 0));

	//

	Events::vehicleDtorEvent.after += [](CVehicle* vehicle)
	{
		if (Vehicles::HasVehicle(vehicle)) Vehicles::RemoveVehicle(vehicle);
	};

	Events::vehicleRenderEvent += [](CVehicle* veh)
	{
		if (Vehicles::HasVehicle(veh)) Vehicles::GetVehicle(veh)->RegisterCoronas();
	};

	Events::drawingEvent += Draw;
	Events::processScriptsEvent += Update;
}

void Mod::Update()
{
	Input::Update();

	Menu::Update();

	//if (Input::GetKey(17)) {
		if (Input::GetKeyDown(48)) { //CTRL + N   78
			ToggleMenu();
		}
	//}

	//

	if (Menu::m_Visible) {
		if (Input::GetKeyDown(18)) {
			SetPlayerControl(true);
			Menu::m_Hide = true;
		}
		if (Input::GetKeyUp(18)) {
			SetPlayerControl(false);
			Menu::m_Hide = false;
		}
	}
	else {
		//if (Vehicles::m_DrawVehicleFrames) Vehicles::m_DrawVehicleFrames = false;
		//if (Vehicles::m_DrawVehiclePoints) Vehicles::m_DrawVehiclePoints = false;
	}

	//

	if (Input::GetKey(17) && Input::GetKeyDown(78))
	{
		auto veh = FindPlayerVehicle(0, false);

		if (veh > 0) {
				
		}
	}

	if (Input::GetKey(17)) {
		if (Input::GetKeyDown(82)) {
			CMessages::AddMessageJumpQ("Config reloaded", 1000, 0, false);
		}
	}

	Vehicles::TryAddAllVehicles();

	for (auto vehicle : Vehicles::GetVehicles())
	{
		vehicle->Update();
	}
}

void Mod::Draw()
{
	CVector2D screenPos(200, 400);
	char buffer[256];

	sprintf(buffer, "Menu windows %d", Menu::m_Windows.size());
	DrawScreenText(buffer, screenPos);

	screenPos.y += 20;

	sprintf(buffer, "Vehicles: %d", Vehicles::GetVehicles().size());
	DrawScreenText(buffer, screenPos);

	screenPos.y += 20;

	sprintf(buffer, "LightGroups:");
	DrawScreenText(buffer, screenPos);

	screenPos.y += 20;

	for (auto lightGroup : LightGroups::GetLightGroups())
	{
		sprintf(buffer, "name= %s, id= %d", lightGroup->name.c_str(), lightGroup->modelId);
		DrawScreenText(buffer, screenPos);
		screenPos.y += 20;
	}

	for (auto vehicle : Vehicles::GetVehicles())
	{
		vehicle->Draw();
	}

	Menu::Draw();
}

void Mod::ToggleMenu()
{
	if (!Menu::m_Visible) {
		WindowMain::m_Vehicle = FindPlayerVehicle(0, false);
		if (!WindowMain::m_Vehicle) {
			Localization::PrintString("error_need_vehicle", 1000);
			return;
		}
	}

	Menu::m_Visible = !Menu::m_Visible;
	Menu::m_Hide = false;

	if (Menu::m_Visible) {
		WindowMain::Create();
		//WindowTest::Create();

		SetPlayerControl(false);
	}
	else {
		Menu::RemoveAllWindows();

		SetPlayerControl(true);

		Config::SaveJSON();
	}
}

void Mod::SetPlayerControl(bool enabled)
{
	Command<Commands::SET_PLAYER_CONTROL>(0, enabled);
}

void Mod::ShowErrorMessage(std::string title, std::string content)
{
	static char buffer[256];
	snprintf(buffer, 256, "%s", content.c_str());

	Log::file << "------------------------------------" << std::endl;
	Log::file << title << std::endl;
	Log::file << content << std::endl;
	Log::file << "------------------------------------" << std::endl;

	if (firstLoad)
	{
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
