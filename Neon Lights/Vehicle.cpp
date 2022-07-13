#include "Vehicle.h"

#include "Pattern.h"
#include "VehicleDummy.h"
#include "Mod.h"

struct CoronaGroup
{
	CVector startPosition;
	std::vector<CVector> offsets;
	float totalDistance = 0.0f;
	float jumpDistance = 0.0f;
	bool invertOffset = false;
	bool keepGlobalOffset = false;

	void UpdateTotalDistance() {
		totalDistance = TotalDistanceBetweenPoints(offsets);
	}
};

int GetAmountOfLightsInLine(float totalDistance, float minDistance)
{
	return (int)floor(totalDistance / minDistance);
}

int GetAmountOfLightsInLine(CVector start, CVector end, float minDistance)
{
	auto totalDistance = DistanceBetweenPoints(start, end);
	return GetAmountOfLightsInLine(totalDistance, minDistance);
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
		DrawWorldText(buffer, lightGroup->dummies[0]->GetPosition(m_Vehicle) + CVector(0, 0, 0.5f));

		for (auto dummy : lightGroup->dummies)
		{
			sprintf(buffer, "dummy~n~%s~n~%s", dummy->name.c_str(), lightGroup->name.c_str());
			DrawWorldText(buffer, dummy->GetPosition(m_Vehicle));
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

		CoronaGroup* currentCoronaGroup = new CoronaGroup();

		coronaGroups.push_back(currentCoronaGroup);

		for (int i = 0; i < (int)lightGroup->dummies.size(); i++)
		{
			auto dummy = lightGroup->dummies[i];

			if (dummy->name == "-")
			{
				currentCoronaGroup = new CoronaGroup();
				currentCoronaGroup->keepGlobalOffset = true;
				coronaGroups.push_back(currentCoronaGroup);

				continue;
			}

			CVector offset = dummy->GetOffset();

			if (currentCoronaGroup->offsets.size() == 0) {
				currentCoronaGroup->startPosition = offset;

				offset.Set(0, 0, 0);
			}
			else {
				offset -= currentCoronaGroup->startPosition;
			}

			if (currentCoronaGroup->offsets.size() == 0 && coronaGroups.size() > 1)
			{
				auto prevCoronaGroup = coronaGroups[coronaGroups.size() - 2];
				auto lastOffset = prevCoronaGroup->offsets[prevCoronaGroup->offsets.size() - 1];


				currentCoronaGroup->jumpDistance = DistanceBetweenPoints(prevCoronaGroup->startPosition + lastOffset, currentCoronaGroup->startPosition + offset);
			}

			currentCoronaGroup->offsets.push_back(offset);

		}

		std::vector<CoronaGroup*> toAdd;

		for (auto clone : lightGroup->clones)
		{
			if (coronaGroups.size() == 0) break;

			clone->toDummy->UpdateOffset(m_Vehicle);

			auto startPosition = clone->toDummy->GetOffset();

			for (auto coronaGroup : coronaGroups)
			{
				auto clonedCoronaGroup = new CoronaGroup();
				clonedCoronaGroup->jumpDistance = coronaGroup->jumpDistance;
				clonedCoronaGroup->invertOffset = clone->invertOffset;

				//

				auto startPositionDiff = (coronaGroup->startPosition - coronaGroups[0]->startPosition);
				if (clone->flipX) {
					startPositionDiff.x = -startPositionDiff.x;
				}
				if (clone->flipY) {
					startPositionDiff.y = -startPositionDiff.y;
				}
				startPosition += startPositionDiff;

				clonedCoronaGroup->startPosition = startPosition;

				//

				for (auto offset : coronaGroup->offsets)
				{
					if (clone->flipX) {
						offset.x = -offset.x;
					}
					if (clone->flipY) {
						offset.y = -offset.y;
					}

					clonedCoronaGroup->offsets.push_back(offset);
				}

				//

				toAdd.push_back(clonedCoronaGroup);
			}
		}
		
		for (auto coronaGroup : toAdd)
		{
			coronaGroups.push_back(coronaGroup);
		}

		float totalDistanceAllGroups = 0.0f;
		for (auto coronaGroup : coronaGroups)
		{
			coronaGroup->UpdateTotalDistance();

			totalDistanceAllGroups += coronaGroup->totalDistance;
		}

		
		for (size_t i = 0; i < coronaGroups.size(); i++)
		{
			auto coronaGroup = coronaGroups[i];

			//Log::file << "[" << i << "] Distance= " << coronaGroup->totalDistance << " of " << totalDistanceAllGroups << "(jump= " << coronaGroup->jumpDistance << ")" << std::endl;
		}

		int globalOffset = 0;

		for (auto coronaGroup : coronaGroups)
		{
			//test
			//add resetGlobalOffset in CoronaGroup
			//only apply when not separating
			//try split S to see what happens
			
			if(!currentCoronaGroup->keepGlobalOffset)
				globalOffset = 0;

			auto amountOfLights = GetAmountOfLightsInLine(coronaGroup->totalDistance, lightGroup->lightsDistance);
			if (lightGroup->lightsDistance == 0) amountOfLights = coronaGroup->offsets.size();

			for (int i = 0; i < amountOfLights; i++)
			{
				float linePosition = coronaGroup->totalDistance / (amountOfLights - 1) * i;

				auto offsetPosition = PointInLines(coronaGroup->offsets, linePosition);
				auto position = VehicleDummy::GetTransformedPosition(m_Vehicle, coronaGroup->startPosition + offsetPosition);

				int offsetBy = lightGroup->offsetBy * (coronaGroup->invertOffset ? -1 : 1);

				int t;
				int stepIndex;
				int offset = (i + globalOffset) * offsetBy;



				//int offset = (i + (coronaGroup->offsetId * lightGroup->amount)) * lightGroup->offsetBy;

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


			globalOffset += amountOfLights;

			if(lightGroup->fixPatternOffset)
			{
				if (coronaGroup->jumpDistance > 0) globalOffset += GetAmountOfLightsInLine(coronaGroup->jumpDistance, lightGroup->lightsDistance);
			}
			
			/*
			for (auto &offset : coronaGroup->offsets)
			{
				auto position = VehicleDummy::GetTransformedPosition(m_Vehicle, offset);

				CCoronas::RegisterCorona(
					lightId++,
					NULL,
					255,
					0,
					0,
					255,
					position,
					0.2f,
					800.0f,
					eCoronaType::CORONATYPE_SHINYSTAR,
					eCoronaFlareType::FLARETYPE_NONE,
					false,
					false,
					0,
					0.0f,
					false,
					0.01f,
					0,
					100.0f,
					false,
					false
				);
			}
			*/
		}


		//

		for (auto coronaGroup : coronaGroups)
		{
			delete coronaGroup;
		}
		coronaGroups.clear();
	}

	//Log::file << "--\n\n\n" << std::endl;
}

void Vehicle::CheckForLightGroups() {
	auto modelId = m_Vehicle->m_nModelIndex;

	for (auto p : LightGroups::m_LightGroups) {
		if (p.first != modelId) continue;

		for (LightGroup* lightGroup : p.second) {
			AddLightGroup(lightGroup);

			for (auto dummy : lightGroup->dummies)
			{
				dummy->UpdateOffset(m_Vehicle);

				//Log::file << "Dummy '" << dummy->name << "' offset=" << FormatCVector(dummy->GetOffset()) << ", pos=" << FormatCVector(dummy->GetPosition(m_Vehicle)) << std::endl;
			}
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