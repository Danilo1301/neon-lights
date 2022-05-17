#include "Config.h"
#include "Mod.h"

std::string Config::m_DataPath = "\\data\\";
std::string Config::m_VehiclesPath = "\\vehicles\\";
std::string Config::m_PatternsPath = "\\patterns\\";

void Config::SaveJSON() {
	Log::file << "[Config] Saving config" << std::endl;

	CreatePath(m_DataPath);
	CreatePath(m_DataPath + m_PatternsPath);
	CreatePath(m_DataPath + m_VehiclesPath);

	DeleteAllConfig();
}


void Config::DeleteAllConfig() {
	Log::file << "[Config] DeleteAllConfig" << std::endl;

	std::filesystem::remove_all(m_DataPath + "settings.json");
}

void Config::LoadJSON() {
	Log::file << "[Config] Loading config" << std::endl;

	CreatePath(m_DataPath);
	CreatePath(m_DataPath + m_PatternsPath);
	CreatePath(m_DataPath + m_VehiclesPath);
	
}

std::string Config::GetFullPath(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;
	return finalPath;
}

void Config::CreatePath(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	if (!std::filesystem::is_directory(finalPath) || !std::filesystem::exists(finalPath)) {
		std::filesystem::create_directory(finalPath);
	}
}

bool Config::Exists(std::string path) {
	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;
	return std::filesystem::exists(finalPath);
}


void Config::WriteToFile(std::string path, Json::Value value) {
	Log::file << "[Config] WriteToFile " << path << std::endl;

	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	Json::StyledWriter writer;
	std::string strJson = writer.write(value);

	std::ofstream file(finalPath);
	file << strJson;
	file.close();
}

Json::Value Config::ReadFile(std::string path) {
	Log::file << "[Config] ReadFile " << path << std::endl;

	std::string pluginPath = PLUGIN_PATH("\\");
	std::string finalPath = pluginPath + path;

	std::ifstream file(finalPath);

	Json::Value value;
	Json::Reader reader;

	if (!reader.parse(file, value, true)) {
		MessageBox(HWND_DESKTOP, std::string("Error loading " + path).c_str(), Mod::m_Name.c_str(), MB_ICONERROR);
	}

	return value;
}
