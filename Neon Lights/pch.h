#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <fstream>
#include <filesystem>

#include "plugin.h"

#include "CPed.h"
#include "RenderWare.h"
#include "CSprite.h"
#include "CFont.h"
#include "common.h"
#include "CRadar.h"
#include "CMenuManager.h"
#include "CFont.h"
#include "CSprite.h"
#include "CTimer.h"
#include "CMessages.h"
#include "CCoronas.h"

#include "extensions/ScriptCommands.h"

#include "json/json.h"

#include "TestHelper.h"

#include "log/Log.h"
#include "input/Input.h"
#include "menu/Menu.h"


#define M_PI 3.14159265358979323846

using namespace plugin;

static float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

static unsigned char ucharIntensity(unsigned char uc, float intensity) {
	return (unsigned char)std::clamp((int)round(((float)uc) * intensity), 0, 255);
}

static void DrawWorldText(char* text, CVector position, CRGBA color = CRGBA(255, 255, 255, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5 * 0.7f, 1.0 * 0.7f);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	RwV3d rwp = { position.x, position.y, position.z };
	RwV3d screenCoors; float w, h;

	CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true);
	CFont::PrintString(screenCoors.x, screenCoors.y, text);
}

static void DrawScreenText(char* text, CVector2D position, CRGBA color = CRGBA(0, 255, 0, 255)) {
	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(color);
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5, 1.0);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);

	CFont::PrintString(position.x, position.y, text);
}

static float distance3d(float x1, float y1, float z1, float x2, float y2, float z2) {
	float d = (float)sqrt(pow(x2 - x1, 2) +
		pow(y2 - y1, 2) +
		pow(z2 - z1, 2) * 1.0);
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	return d;
}

static char* (*GetFrameNodeName)(RwFrame* frame) = (char* (*)(RwFrame*))0x72FB30;

static CVector GetFrameNodePosition(RwFrame* frame) {
	return { frame->modelling.pos.x, frame->modelling.pos.y, frame->modelling.pos.z };
}

static CVector CVectorLerp(CVector A, CVector B, float t) {
	return A * t + B * (1.f - t);
}

static CRGBA CRGBALerp(CRGBA color1, CRGBA color2, float t) {
	CRGBA color;
	color.r = (unsigned char)(color1.r + ((color2.r - color1.r) * t));
	color.g = (unsigned char)(color1.g + ((color2.g - color1.g) * t));
	color.b = (unsigned char)(color1.b + ((color2.b - color1.b) * t));
	color.a = 255;
	return color;
}

static std::string FormatCVector(CVector vec) {
	return std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z);
}

static std::string StringToUpper(std::string data) {
	std::for_each(data.begin(), data.end(), [](char& c) {
		c = ::toupper(c);
	});
	return data;
}