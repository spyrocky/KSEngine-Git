#pragma once

#include "SDL2/SDL.h"

class Input {
public:
	Input();

	//detect the input of the user
	void ProcessInput();

	bool IsKeyDown(SDL_Scancode Key);


private:
	//detect if a specific key is down base on sdl scancode 
	//Hint * SDL_SCANCODE_KEY (SDL_SCANCODE_W)
	const Uint8* KeyboardState;


};