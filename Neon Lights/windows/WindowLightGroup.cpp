#include "WindowLightGroup.h"
#include "WindowMain.h"

#include "../Mod.h"
#include "../Patterns.h"
#include "../Vehicles.h"
#include "../menu/Menu.h"
#include "../menu/PositionEditor.h"
#include "../menu/TextEditor.h"

#include "../localization/Localization.h"

LightGroup* WindowLightGroup::m_LightGroup = nullptr;

void WindowLightGroup::Create() {
	auto veh = WindowMain::m_Vehicle;
	auto window = Menu::AddWindow(Mod::m_Name, "LightGroups");

	auto buttonAddLightGroupSiren = window->AddButton(Localization::GetLine("add_light_group"));
	buttonAddLightGroupSiren->m_OnClick = [veh, window]() mutable {

		auto lightGroup = LightGroups::CreateLightGroup(veh->m_nModelIndex);
		lightGroup->position = CVector(0, 0, 2);

		lightGroup->AddPoint(CVector(-1, 0, 0));
		lightGroup->AddPoint(CVector(1, 0, 0));

		Menu::RemoveWindow(window);
		Create();
	};


	auto lightGroups = LightGroups::GetLightGroups();

	window->AddItem("---------- " + std::to_string(lightGroups.size()) + " LightGroups ----------");

	for (auto lightGroup : lightGroups)
	{
		auto button1 = window->AddButton(Localization::GetLineFormatted("edit_light_group", lightGroup->name));
		button1->m_OnClick = [window, lightGroup]() mutable {
			Menu::RemoveWindow(window);

			m_LightGroup = lightGroup;
			CreateEditLightGroup();
		};
	}

	window->AddItem("--------------------------------------");

	auto buttonBack = window->AddButton(Localization::GetLine("back"));
	buttonBack->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		WindowMain::Create();
	};
}

void WindowLightGroup::CreateEditLightGroup() {
	auto lightGroup = m_LightGroup;
	CVehicle* veh = WindowMain::m_Vehicle;

	auto window = Menu::AddWindow("Vehicle Siren Lights", "LightGroups > " + lightGroup->name);

	auto buttonName = window->AddButton(Localization::GetLine("edit_name"));
	buttonName->AddTextStr(&lightGroup->name, CVector2D(10, 0));
	buttonName->m_OnClick = [lightGroup]() {
		TextEditor::Open(Localization::GetLine("edit_name"), true, &lightGroup->name);
	};

	auto buttonEditPoints = window->AddButton(Localization::GetLineFormatted("edit_points", lightGroup->points.size()));
	buttonEditPoints->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		//WindowPoint::CreatePoints();
	};

	auto positionButton = window->AddButton(Localization::GetLine("edit_position_points"));
	positionButton->m_OnClick = [lightGroup]() {
		PositionEditor::Toggle(&lightGroup->position);
	};

	auto amountLights = window->AddNumberRange("Amount of lights", &lightGroup->amountLights, 1, 9999);

	auto buttonDelete = window->AddButton(Localization::GetLine("delete"));
	buttonDelete->m_BackgroundColor = CRGBA(255, 0, 0, 80);
	buttonDelete->m_BackgroundColorSelected = CRGBA(255, 0, 0, 172);
	buttonDelete->m_OnClick = [window, lightGroup]() {
		Vehicles::RemoveAllVehicles();
		//LightGroups::RemoveLightGroup(lightGroup);
		Vehicles::TryAddAllVehicles();

		Menu::RemoveWindow(window);
		Create();
	};

	auto buttonBack = window->AddButton(Localization::GetLine("back"));
	buttonBack->m_OnClick = [window]() {
		Menu::RemoveWindow(window);
		Create();
	};
}