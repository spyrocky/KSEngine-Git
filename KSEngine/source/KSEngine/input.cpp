#include "KSEngine/input.h"
#include "KSEngine/Game.h"
#include <iostream>

using namespace std;
Input::Input()
{
    // Assign the current key by default

    KeyboardState = SDL_GetKeyboardState(NULL);
}

void Input::ProcessInput()
{
    //creating event for game loop to close when a key is pressed
    SDL_Event PollEvent;

    //while the loop is waiting for input
    while (SDL_PollEvent(&PollEvent)) {
        //checking what input was pressed
        switch (PollEvent.type) {
        case SDL_KEYDOWN:
            // update all key on keyboard with up or down state
            KeyboardState = SDL_GetKeyboardState(NULL);
            break;
        case SDL_KEYUP:
            KeyboardState = SDL_GetKeyboardState(NULL);
            break;
        case SDL_QUIT:      //close button clicked
            Game::GetGameInstance().CloseApp();
            break;
        default:
            break;
        }
    }
}

bool Input::IsKeyDown(SDL_Scancode Key)
{
    //make sure the keyboard state is set
    if (KeyboardState == nullptr) {
        cout << "No keyboard state detected." << endl;
        return false;
    }

    //return the result of the key
	return static_cast<bool>(KeyboardState[Key]);
}
