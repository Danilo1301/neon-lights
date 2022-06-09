#include "Vehicle.h"

#include "Pattern.h"
#include "VehicleDummy.h"
#include "Mod.h"

struct CoronaGroup
{
	CVector startPosition = CVector(0, 0, 0);
	std::vector<CVector> offsets;
};

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

	while (distance < position && point < (int)points.size())
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

Vehicle::Vehicle(CVehicle* vehicle) {
	m_Vehicle = vehicle;

	CheckForLightGroups();
}

void Vehicle::Update() {

	for (LightGroupData* lightGroupData : m_LightGroupData) {
		auto lightGroup = lightGroupData->lightGroup;

		lightGroupData->patternProgress += 10;
		if (lightGroupData->patternProgress >= GetPatternMaxTime(lightGroup->pattern)) {
			lightGroupData->patternProgress = 0;
		}
	}
}

void Vehicle::Draw() {
}

void Vehicle::DrawDebug() {
	static char buffer[512] = "";

	for (LightGroupData* lightGroupData : m_LightGroupData) {
		auto lightGroup = lightGroupData->lightGroup;

		sprintf(buffer, "%s~n~%d", lightGroup->name.c_str(), lightGroupData->patternProgress);
		DrawWorldText(buffer, lightGroup->dummies[0]->GetTransformedPosition(m_Vehicle) + CVector(0, 0, 0.5f));

		for (auto dummy : lightGroup->dummies)
		{
			sprintf(buffer, "dummy~n~%s~n~%s", dummy->name.c_str(), lightGroup->name.c_str());
			DrawWorldText(buffer, dummy->GetTransformedPosition(m_Vehicle));
		}
	}
}

void Vehicle::RegisterCoronas() {
	if (!m_Enabled) return;;

	int lightId = reinterpret_cast<unsigned int>(m_Vehicle) + 20;

	//Log::file << "RegisterCoronas" << std::endl;

	for (LightGroupData* lightGroupData : m_LightGroupData)
	{
		auto lightGroup = lightGroupData->lightGroup;

		Pattern* pattern = lightGroup->pattern;

		//
		std::vector<CoronaGroup*> coronaGroups;
		std::vector<CoronaGroup*> mainCoronaGroups;

		CoronaGroup* coronaGroup = NULL;

		CVector mainStartPosition = CVector(0, 0, 0);
		if (lightGroup->dummies.size() > 0) mainStartPosition = lightGroup->dummies[0]->GetTransformedPosition(m_Vehicle);

		//Log::file << "[LIGHT GROUP DATA]" << std::endl;

		for (int i = 0; i < (int)lightGroup->dummies.size(); i++)
		{
			Dummy* dummy = lightGroup->dummies[i];

			if (dummy->name == "-")
			{
				coronaGroup = NULL;
				continue;
			}

			if (coronaGroup == NULL)
			{
				coronaGroup = new CoronaGroup();
				coronaGroup->startPosition = mainStartPosition;
				coronaGroups.push_back(coronaGroup);

				mainCoronaGroups.push_back(coronaGroup);
			}

			CVector position = dummy->GetTransformedPosition(m_Vehicle);
			CVector offset = coronaGroup->startPosition - position;
			coronaGroup->offsets.push_back(offset);

			//Log::file << "[dummy offset] " << FormatCVector(offset) << std::endl;
		}

		
		//coronaGroups.push_back(mainCoronaGroup);

		//

		
		for (auto clone : lightGroup->clones)
		{

			for (auto cg : mainCoronaGroups)
			{
				CoronaGroup* coronaGroup = new CoronaGroup();
				coronaGroup->startPosition = clone->toDummy->GetTransformedPosition(m_Vehicle);
		
				for (auto offset : cg->offsets)
				{
					if (clone->flipX) offset.x = -offset.x;
					if (clone->flipY) offset.y = -offset.y;

					coronaGroup->offsets.push_back(offset);
				}

				coronaGroups.push_back(coronaGroup);
			}
		}
		

		//


		for (auto coronaGroup : coronaGroups)
		{

			float totalDistance = TotalDistanceBetweenPoints(coronaGroup->offsets);
			for (int i = 0; i < lightGroup->amount; i++)
			{
				float linePosition = totalDistance / (lightGroup->amount - 1) * i;

				CVector offsetPosition = PointInLines(coronaGroup->offsets, linePosition);
				CVector position = coronaGroup->startPosition - offsetPosition;

				//
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
					NULL,
					color.r,
					color.g,
					color.b,
					color.a,
					position,
					lightGroup->size,
					lightGroup->farClip,
					eCoronaType::CORONATYPE_SHINYSTAR,
					eCoronaFlareType::FLARETYPE_NONE,
					false,
					false,
					0,
					0.0f,
					false,
					lightGroup->nearClip,
					0,
					400.0f,
					false,
					false
				);
			}

			//draws offset points
			/*
			Log::file << "--coronaGroup--" << std::endl;
			Log::file << "start= " << FormatCVector(coronaGroup->startPosition) << std::endl;
			Log::file << "offsets= " << std::to_string(coronaGroup->offsets.size()) << std::endl;
			for (int i = 0; i < (int)coronaGroup->offsets.size(); i++)
			{
				Log::file << "offset [" << i << "] " << FormatCVector(coronaGroup->offsets[i]) << std::endl;
			}
			Log::file << "-- -- --" << std::endl;

			float totalDistance = TotalDistanceBetweenPoints(coronaGroup->offsets);
			for (int i = 0; i < (int)coronaGroup->offsets.size(); i++)
			{
				//CVector offset = coronaGroup.offsets[i];

				float linePosition = totalDistance / (coronaGroup->offsets.size() - 1) * i; // add size -1 maybe

				CVector offsetPosition = PointInLines(coronaGroup->offsets, linePosition);
				Log::file << "drawing [" << i << "] offset " << FormatCVector(offsetPosition) << ", dist=" << totalDistance << ", pos = " << linePosition << std::endl;

				CVector position = coronaGroup->startPosition - offsetPosition;

				Log::file << "at " << FormatCVector(position) << std::endl;

				CRGBA color = { 255, 0, 0, 255 };

				if (i == 0)
				{
					color = { 0, 0, 255, 255 };
				}

				CCoronas::RegisterCorona(
					lightId++,
					NULL,
					color.r,
					color.g,
					color.b,
					color.a,
					position,
					lightGroup->size,
					lightGroup->farClip,
					eCoronaType::CORONATYPE_SHINYSTAR,
					eCoronaFlareType::FLARETYPE_NONE,
					false,
					false,
					0,
					0.0f,
					false,
					lightGroup->nearClip,
					0,
					200.0f,
					false,
					false
				);
			}
			*/
		}

		for (auto c : coronaGroups) delete c;
		coronaGroups.clear();
	}

	Log::file << "--\n\n\n" << std::endl;
}

void Vehicle::CheckForLightGroups() {
	auto modelId = m_Vehicle->m_nModelIndex;

	for (auto p : LightGroups::m_LightGroups) {
		if (p.first != modelId) continue;

		for (LightGroup* lightGroup : p.second) {
			AddLightGroup(lightGroup);
		}
	}
}

void Vehicle::Destroy() {
	for (LightGroupData* lightGroupData : m_LightGroupData) {
		delete lightGroupData;
	}
	m_LightGroupData.clear();
};

void Vehicle::AddLightGroup(LightGroup* lightGroup) {
	LightGroupData* lightGroupData = new LightGroupData();
	lightGroupData->lightGroup = lightGroup;
	m_LightGroupData.push_back(lightGroupData);
}