#include "LightGroup.h"

LightGroup::LightGroup(int modelId)
{
	this->modelId = modelId;
}

Point* LightGroup::AddPoint(CVector offset)
{
	Point* point = new Point();
	point->offset = offset;
	points.push_back(point);
	return point;
}

CVector LightGroup::GetPointPosition(Point* point, CVehicle* vehicle)
{
	auto offset = this->position + point->GetOffset();
	auto position = VehicleDummy::GetTransformedPosition(vehicle, offset);

	return position;
}