#include "pch.h"

class NeonLights {
public:
    NeonLights();

    static bool m_EnableDebug;

    static void LoadConfig();
    static void ShowErrorMessage(std::string title, std::string content);
};