#pragma once
#include "KSEngine/CoreMinimal.h"

class Game {
public:
	// Get the game instance or create one if it doesn't exist
	static Game& GetGameInstance();

	// Destroy the game running the destructor
	static void DestroyGameInstance();

	// Start the game/app
	// Load the window
	void Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);

private:
	Game();
	~Game();

	// Run the game loop
	void Run();

	// Handle the Input
	void ProcessInput();

	// Handle Game Logic
	void Update();

	// Handle the 3D graphics drawing to the screen
	// Will clear and present each frame
	void Draw();

	// Handle anything that needs to be deleted when the game ends
	void CloseGame();

private:
	// boolean that ends the game
	bool bIsGameOver;

	// shared pointer to the Graphics class
	GraphicsEnginePtr Graphics;
};