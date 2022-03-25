#include "pch.h"
#include "KeyPress.h"

std::vector<int> keysPressed;
std::vector<int> keysDown;
std::vector<int> keysUp;

void KeyPress::UpdateKeyState(int key) {
	if (plugin::KeyPressed(key)) {
		if (std::find(keysPressed.begin(), keysPressed.end(), key) == keysPressed.end()) {
			keysPressed.push_back(key);
			keysDown.push_back(key);
		}
	}
	else {
		if (std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end()) {
			keysPressed.erase(std::find(keysPressed.begin(), keysPressed.end(), key));
			keysUp.push_back(key);
		}
	}
}

bool KeyPress::GetKey(int key) {
	UpdateKeyState(key);
	return std::find(keysPressed.begin(), keysPressed.end(), key) != keysPressed.end();
}

bool KeyPress::GetKeyDown(int key) {
	UpdateKeyState(key);
	return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
}

bool KeyPress::GetKeyUp(int key) {
	UpdateKeyState(key);
	return std::find(keysUp.begin(), keysUp.end(), key) != keysUp.end();
}

void KeyPress::Update() {
	keysDown.clear();
	keysUp.clear();
}