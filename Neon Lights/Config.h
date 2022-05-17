#pragma once

#include "pch.h"

class Config {
public:
	static std::string m_DataPath;
	static std::string m_VehiclesPath;
	static std::string m_PatternsPath;

	static void SaveJSON();
	static void DeleteAllConfig();
	static void LoadJSON();

	static std::string GetFullPath(std::string path);
	static void CreatePath(std::string path);
	static bool Exists(std::string path);

	static void WriteToFile(std::string path, Json::Value value);
	static Json::Value ReadFile(std::string path);
};