#include "Patterns.h"

std::vector<Pattern*> Patterns::m_Patterns;

Pattern* Patterns::CreatePattern(std::string name)
{
	Pattern* pattern = new Pattern(name);
	pattern->name = name;
	m_Patterns.push_back(pattern);
	return pattern;
}

Pattern* Patterns::GetPattern(std::string name)
{
	for (auto p : m_Patterns) {
		if (name.compare(p->name) == 0) return p;
	}
	return nullptr;
}

void Patterns::RemovePatterns()
{
	for (auto pattern : m_Patterns) {
		for (auto step : pattern->steps) {
			delete step;
		}
		pattern->steps.clear();
		delete pattern;
	}
	m_Patterns.clear();
}