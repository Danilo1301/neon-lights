#include "pch.h"

class NeonLights {
public:
    NeonLights();

    static bool m_EnableDebug;

    static void LoadConfig();
    static void ShowErrorMessage(std::string title, std::string content);

    template<class T>
    static Json::Value ValidateValue(Json::Value value, T defaultValue);
};