#pragma once

#include "bitmap.h"
#include "rectangle.h"

typedef enum {
	NORMAL_MOUSE, NORMAL_KEYBOARD, NIGHT_MOUSE, NIGHT_KEYBOARD, EXIT
} ModeAction;

typedef struct {
	int done;
	Bitmap* background;
	Bitmap* NormalMouse;
	Bitmap* onNormalMouse;
	Bitmap* NormalKeyboard;
	Bitmap* onNormalKeyboard;
	Bitmap* NightMouse;
	Bitmap* onNightMouse;
	Bitmap* NightKeyboard;
	Bitmap* onNightKeyboard;
	int mouseOnNormalMouse;
	Rectangle* NormalMouseButton;
	int mouseOnNormalKeyboard;
	Rectangle* NormalKeyboardButton;
	int mouseOnNightMouse;
	Rectangle* NightMouseButton;
	int mouseOnNightKeyboard;
	Rectangle* NightKeyboardButton;

	int action;
} ModeState;

ModeState* newModeState();
int updateModeState(ModeState* state, unsigned long scancode);
void drawModeState(ModeState* state);
void deleteModeState(ModeState* state);
