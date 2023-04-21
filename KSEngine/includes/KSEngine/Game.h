#pragma once
#include "CoreMinimal.h"

class Input;

class Game {
public:
	//Get game instance or create if it doesn't exist
	static Game& GetGameInstance();

	//Destroy the game runnning destructor
	static void DestroyGameInstance();

	//starts the game
	//loading the window
	void Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);

	//get precise delta time
	double GetDeltaTime() { return DeltaTime; }

	//get less precise delta time
	float GetFDeltaTime() { return static_cast<float>(DeltaTime); }

	//return the graphics engine
	GraphicsEnginePtr GetGraphics() { return Graphics; }
	//set bIsGameOver to true
	void CloseApp() { bIsGameOver = true; }

	// return the graphic engine default texture
	TexturePtr GetDefaultGEngineTexture();

	MaterialPtr GetDefaultGEngineMaterial();

private:
	Game();
	~Game();

	//run the game loop
	void Run();

	//handle I/O
	void ProcessInput();

	//handle game logic
	void Update();

	//handle the 3D rendering
	//will clear and present each frame
	void Draw();

	//handle anything that needs to be deleted when the game ends
	void CloseGame();



private:
	//boolean that ends the game
	bool bIsGameOver;

	//shared pointer to the graphics class
	GraphicsEnginePtr Graphics;

	//the time between each frame
	double DeltaTime;

	//read the input of the player
	Input* GameInput;

	//temporary mesh variables
	ModelPtr Model3;
	ModelPtr Model2;
	ModelPtr Model;

	//ModelPtr Wall2;
	ModelPtr Wall;
};