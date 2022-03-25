#include "VehicleDummies.h"

//static std::map<int, std::vector<RwFrame*>> frames;

std::vector<RwFrame*> frames;

std::vector<RwFrame*> VehicleDummies::GetFramesOnVehicle(CVehicle* vehicle) {
	frames.clear();

	/*
	if (frames.find(vehicle->m_nModelIndex) == frames.end()) {
		frames.insert(std::pair<int, std::vector<RwFrame*>>(vehicle->m_nModelIndex, {}));

		FindDummies(vehicle, (RwFrame*)vehicle->m_pRwClump->object.parent);

		//log_file << "Found " << frames[vehicle->m_nModelIndex].size() << " frames on vehicle id " << vehicle->m_nModelIndex << std::endl;
	}
	*/

	FindDummies(vehicle, (RwFrame*)vehicle->m_pRwClump->object.parent);

	return frames;
}

CVector VehicleDummies::GetDummyPosition(RwFrame* frame) {
	return { frame->modelling.pos.x, frame->modelling.pos.y, frame->modelling.pos.z };
}

void VehicleDummies::FindDummies(CVehicle* vehicle, RwFrame* frame, bool parent) {
	static char text[256];

	if (!frame)
		return;

	if (RwFrame* nextFrame = frame->child)
		FindDummies(vehicle, nextFrame, (RwFrameGetParent(frame)) ? (true) : (false));

	if (RwFrame* nextFrame = frame->next)
		FindDummies(vehicle, nextFrame, parent);

	if (std::find(frames.begin(), frames.end(), frame) != frames.end())
		return;

	frames.push_back(frame);
	/*
	if (std::find(frames[vehicle->m_nModelIndex].begin(), frames[vehicle->m_nModelIndex].end(), frame) != frames[vehicle->m_nModelIndex].end())
		return;

	frames[vehicle->m_nModelIndex].push_back(frame);
	*/


	//const std::string name = GetFrameNodeName(frame);
	//log_file << "* Added " << name << ", size=" << frames[vehicle->m_nModelIndex].size() << std::endl;
}