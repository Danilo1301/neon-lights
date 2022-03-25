#pragma once

#include "pch.h"

struct PatternStep {
	CRGBA color;
	int time;
};

class Pattern {
public:
	std::string name;
	std::vector<PatternStep*> steps;

	Pattern(std::string name) {
		this->name = name;
	}

	void AddColor(CRGBA color, int time) {
		PatternStep* step = new PatternStep();
		step->color = color;
		step->time = time;
		steps.push_back(step);
	}
};

class Patterns {
public:
	static std::vector<Pattern*> m_Patterns;

	static Pattern* CreatePattern(std::string name);
	static Pattern* GetPattern(std::string name);
	static void RemovePatterns();
};