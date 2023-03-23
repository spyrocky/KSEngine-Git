#pragma once
#include "SDL2/SDL.h"

enum MouseButtons {
	LEFT = SDL_BUTTON_LEFT,
	MIDDLE = SDL_BUTTON_MIDDLE,
	RIGHT = SDL_BUTTON_RIGHT

};

class Input {
public:
	Input();

	//detect the input of the user
	void ProcessInput();

	//detect if a specific key is down based on an sdl scancode
	//SDL_SCANCODE_*KEY*
	bool IsKeyDown(SDL_Scancode Key);
	//detect if the mouse button inputs is being pressed down
	bool IsMouseButtonDown(MouseButtons Button);

private:
	//update the mouse button state with the current state mouse
	void SetMouseButtonStates(Uint8 ButtonIndex, bool CurrentState);

	// handle the varibles that rely on mouse motion
	void onMouseMove(SDL_MouseMotionEvent& MEvent);

	// handle the varibles that rely on mouse scroll
	void onMouseScroll(SDL_MouseWheelEvent& MEvent);

public:
	// Mouse position
	int MouseX, MouseY;

	// How far the mouse has move since last  check/tick
	int MouseXDelta, MouseYDelta;

	// how far the player has scrolled since the last frame
	float ScrollDelta;

private:

	//keyboard current state
	const Uint8* KeyboardState;

	// mouse button current state
	bool MouseButtonStates[10] = { false };

};