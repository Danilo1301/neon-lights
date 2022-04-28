#include "VehicleInfo.h"
#include "Pattern.h"
#include "VehicleDummies.h"
#include "Log.h"
#include "NeonLights.h"

float TotalDistanceBetweenPoints(std::vector<CVector> points) {
	float totalDistance = 0.0f;
	for (size_t i = 0; i < points.size() - 1; i++)
		totalDistance += DistanceBetweenPoints(points[i], points[i + 1]);
	return totalDistance;
}

int GetPatternMaxTime(Pattern* pattern) {
	int time = 0;
	for (PatternStep* step : pattern->steps) time += step->time;
	return time;
}

void GetPatternStepAndTime(Pattern* pattern, int time, int& outTime, int& outStep) {
	int maxTime = GetPatternMaxTime(pattern);

	static char buffer[512];

	int t = (int)((float)time - (floor((float)time / (float)maxTime) * (float)maxTime));

	//sprintf(buffer, "time %d; t %d; max %d", time, t, maxTime);
	//MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);

	outTime = 0;

	int findTime = 0;
	int stepIndex = -1;
	while (findTime < t) //&& stepIndex < pattern->steps.size()
	{
		stepIndex++;
		outTime = t - findTime;


		sprintf(buffer, "time %d; t %d; findTime %d; stepIndex %d", time, t, findTime, stepIndex);
		//MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);

		findTime += pattern->steps[stepIndex]->time;
		
	}

	if (stepIndex == -1) stepIndex = 0;

	outStep = stepIndex;
}

CVector PointInLines(std::vector<CVector> points, float position) {
	static char buffer[512];

	float totalDistance = TotalDistanceBetweenPoints(points);

	float distance = 0.0f;
	float toLerp = 0.0f;

	int point = -1;

	while (distance < position && point < (int)points.size() - 1)
	{
		point++;

		float len = DistanceBetweenPoints(points[point], points[point + 1]);

		toLerp = (position - distance) / len;
		
		//sprintf(buffer, "point %d; position %.2f; distance %.2f; len %.2f; toLerp %.2f", point, position, distance, len, toLerp);
		//MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);

		distance += len;
	}

	if (point == -1) return points[0];

	return CVectorLerp(points[point + 1], points[point], toLerp);
}

VehicleInfo::VehicleInfo(CVehicle* vehicle, int modelId) {
	m_Vehicle = vehicle;

	std::vector<LightGroup*> lightGroups;
	std::vector<Dummy*> dummies;

	for (auto p : LightGroups::m_LightGroups) {
		if (p.first != modelId) continue;

		for (LightGroup* lightGroup : p.second) {
			lightGroups.push_back(lightGroup);

			for (Dummy* dummy : lightGroup->dummies) {
				if (dummy->name.empty()) continue;
				dummies.push_back(dummy);
			}
		}
	}

	if (dummies.size() > 0) {
		auto frames = VehicleDummies::GetFramesOnVehicle(vehicle);

		for (auto frame : frames) {

			for (Dummy* dummy : dummies) {
				std::string frameName = GetFrameNodeName(frame);

				if (frameName.compare(dummy->name) == 0)
					dummy->position = VehicleDummies::GetDummyPosition(frame);
			}
		}
	}

	for (LightGroup* lightGroup : lightGroups) {
		AddLightGroup(lightGroup);
	}
}

void VehicleInfo::Update() {
	for (LightGroupData* lightGroupData : m_LightGroupData) {
		auto lightGroup = lightGroupData->lightGroup;

		lightGroupData->patternProgress += 10;
		if (lightGroupData->patternProgress >= GetPatternMaxTime(lightGroup->pattern)) {
			lightGroupData->patternProgress = 0;
		}
	}
}

void VehicleInfo::Draw() {
	
	static char buffer[512] = "";

	if (!NeonLights::m_EnableDebug) return;

	for (LightGroupData* lightGroupData : m_LightGroupData) {
		auto lightGroup = lightGroupData->lightGroup;

		sprintf(buffer, "%s~n~%d", lightGroup->name.c_str(), lightGroupData->patternProgress);
		DrawWorldText(buffer, m_Vehicle->TransformFromObjectSpace(lightGroup->dummies[0]->position + CVector(0, 0, 0.5f)));

		for (auto dummy : lightGroup->dummies)
		{
			sprintf(buffer, "dummy~n~%s~n~%s", dummy->name.c_str(), lightGroup->name.c_str());
			DrawWorldText(buffer, m_Vehicle->TransformFromObjectSpace(dummy->position));
		}
	}

	/*
	for (LightGroupData* lightGroupData : m_LightGroupData) {
		auto lightGroup = lightGroupData->lightGroup;

		sprintf(buffer, "%s~n~%d", lightGroup->name.c_str(), lightGroupData->patternProgress);
		DrawWorldText(buffer, m_Vehicle->TransformFromObjectSpace(lightGroup->dummies[0]->position + CVector(0, 0, 1.0f)));

		
		if ((int)lightGroupData->lightStoredPositions.size() > 0) {

			for (int i = 0; i < lightGroup->amount; i++) {
				auto position = m_Vehicle->TransformFromObjectSpace(lightGroupData->lightStoredPositions[i]);

				int t;
				int stepIndex;
				int offset = (i * lightGroup->offsetBy);

				GetPatternStepAndTime(
					lightGroup->pattern,
					lightGroupData->patternProgress + offset,
					t,
					stepIndex
				);

				PatternStep* step = lightGroup->pattern->steps[stepIndex];

				//sprintf(buffer, "%d~n~%s~n~%d~n~%d", i, lightGroup->name.c_str(), t, step->time);
				//MessageBox(HWND_DESKTOP, buffer, "", MB_ICONERROR);
				//DrawWorldText(buffer, position);
				//float currentProgress = GetPatternProgress(lightGroup->pattern, lightGroupData->patternProgress + (i * 0.2f));
			}
		}
	}
	*/
}

void VehicleInfo::RegisterCoronas() {

	if (!m_Enabled) return;

	for (LightGroupData* lightGroupData : m_LightGroupData) {

		if (lightGroupData->lightStoredPositions.size() == 0) {
			auto lightGroup = lightGroupData->lightGroup;

			std::vector<CVector> points;
			for (Dummy* dummy : lightGroup->dummies) points.push_back(dummy->position);
			float totalDistance = TotalDistanceBetweenPoints(points);

			for (int i = 0; i < lightGroup->amount; i++) {
				float linePosition = totalDistance / (lightGroup->amount - 1) * i;

				CVector position = PointInLines(points, linePosition);
				lightGroupData->lightStoredPositions.push_back(position);
			}
		}
	}

	int lightId = reinterpret_cast<unsigned int>(m_Vehicle) + 20;

	for (LightGroupData* lightGroupData : m_LightGroupData) {
		LightGroup* lightGroup = lightGroupData->lightGroup;
		Pattern* pattern = lightGroup->pattern;

		for (int i = 0; i < (int)lightGroupData->lightStoredPositions.size(); i++)
		{
			int t;
			int stepIndex;
			int offset = (i * lightGroup->offsetBy);

			GetPatternStepAndTime(
				lightGroup->pattern,
				lightGroupData->patternProgress + offset,
				t,
				stepIndex
			);

			PatternStep* step = pattern->steps[stepIndex];
			PatternStep* nextStep = pattern->steps[(stepIndex + 1) % pattern->steps.size()];

			CRGBA color = step->color;

			if (lightGroup->lerpColor) {
				float lerpProgress = (float)t / (float)step->time;

				color = CRGBALerp(
					step->color,
					nextStep->color,
					lerpProgress
				);
			}
			
			CCoronas::RegisterCorona(
				lightId++,
				m_Vehicle,
				color.r,
				color.g,
				color.b,
				color.a,
				lightGroupData->lightStoredPositions[i],
				lightGroup->size,
				lightGroup->farClip,
				eCoronaType::CORONATYPE_SHINYSTAR,
				eCoronaFlareType::FLARETYPE_NONE,
				false,
				false,
				0,
				0.0f,
				false,
				lightGroup->nearClip, //0.1
				0,
				15.0f,
				false,
				false
			);
		}
	}

	/*
	for (LightGroupData* lightGroupData : m_LightGroupData) {
		for (Dummy* dummy : lightGroupData->lightGroup->dummies) {
			RegisterCorona(lightId++, dummy->position, CRGBA(255, 0, 0), 0.1f);
		}
	}
	*/
}

void VehicleInfo::Destroy() {
	for (LightGroupData* lightGroupData : m_LightGroupData) {
		delete lightGroupData;
	}
	m_LightGroupData.clear();
};

void VehicleInfo::AddLightGroup(LightGroup* lightGroup) {
	LightGroupData* lightGroupData = new LightGroupData();
	lightGroupData->lightGroup = lightGroup;
	m_LightGroupData.push_back(lightGroupData);
}