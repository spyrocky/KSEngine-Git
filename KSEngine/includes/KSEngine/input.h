#pragma once
#include "SDL2/SDL.h"

class Input {
public:
	Input();

	//detect the input of the user
	void ProcessInput();

	//detect if a specific key is down based on an sdl scancode
	//SDL_SCANCODE_*KEY*
	bool IsKeyDown(SDL_Scancode Key);

private:
	const Uint8* KeyboardState;

};