#pragma once

class KeyPress {
private:
	static void UpdateKeyState(int key);
public:
	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);

	static void Update();
};