#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <Windows.h>

void onKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
void updateKeyboard();

bool wasKeyPressed(DWORD key);
bool isKeyDown(DWORD key);
bool wasKeyReleased(DWORD key);

#endif // !KEYBOARD_H