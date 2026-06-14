#include "keyboard.h"

constexpr auto KEY_STATE_NONE = 0;
constexpr auto KEY_STATE_PRESSED = 1;
constexpr auto KEY_STATE_DOWN = 2;
constexpr auto KEY_STATE_RELEASED = 3;

constexpr auto KEYS_ARRAY_SIZE = 256;

int keys[KEYS_ARRAY_SIZE];

void onKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	// If is released, then assign the state as released, otherwise assign it as pressed
	keys[key] = (wasDownBefore && isUpNow) ? KEY_STATE_RELEASED : KEY_STATE_PRESSED;
}

void updateKeyboard()
{
	for (int i = 0; i < KEYS_ARRAY_SIZE; ++i)
	{
		int* state = &keys[i];

		if (*state == KEY_STATE_PRESSED)
		{
			*state = KEY_STATE_DOWN;
		}
		else if (*state == KEY_STATE_RELEASED)
		{
			*state = KEY_STATE_NONE;
		}
	}
}

bool wasKeyPressed(DWORD key)
{
	return key < KEYS_ARRAY_SIZE && keys[key] == KEY_STATE_PRESSED;
}

bool isKeyDown(DWORD key)
{
	return key < KEYS_ARRAY_SIZE && (keys[key] == KEY_STATE_PRESSED || keys[key] == KEY_STATE_DOWN);
}

bool wasKeyReleased(DWORD key)
{
	return key < KEYS_ARRAY_SIZE && keys[key] == KEY_STATE_RELEASED;
}