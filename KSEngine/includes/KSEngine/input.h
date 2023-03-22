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


private:

	//keyboard current state
	const Uint8* KeyboardState;

	// mouse button current state
	bool MouseButtonStates[3] = { false };

};