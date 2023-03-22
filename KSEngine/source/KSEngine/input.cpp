#include "KSEngine/input.h"
#include "KSEngine/Game.h"
#include <iostream>

Input::Input()
{
    //assign the current keys by default
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
        case SDL_MOUSEBUTTONDOWN:
            // PollEvent.button.button gets the index of the mpuse button pressed
            SetMouseButtonStates(PollEvent.button.button, true);
            break;
        case SDL_MOUSEBUTTONUP:
            SetMouseButtonStates(PollEvent.button.button, false);
            break;
        case SDL_MOUSEMOTION:
            //SetMouseButtonStates(PollEvent.button.button, true);
            break;
        case SDL_MOUSEWHEEL:
            //SetMouseButtonStates(PollEvent.button.button, true);
            break;
        case SDL_KEYDOWN:
            //update all the keys on the keyboard with up or down states
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
        std::cout << "No keyboard state detected." << endl;
        return false;
    }

    return static_cast<bool>(KeyboardState[Key]);}

bool Input::IsMouseButtonDown(MouseButtons Button)
{
    return false;
}

void Input::SetMouseButtonStates(Uint8 ButtonIndex, bool CurrentState)
{
    MouseButtonStates[ButtonIndex] = CurrentState;
}
