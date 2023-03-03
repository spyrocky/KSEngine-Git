#include "KSEngine/Game.h"
#include "KSEngine/Graphics/GraphicsEngine.h"

Game& Game::GetGameInstance()
{
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();

	delete GameInstance;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{

	Graphics = make_shared<GraphicsEngine>();

	if (!Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight)) {
		bIsGameOver = true;
	}

	Run();
}

/*
int Game::GameMenu()
{
	int inputkey = 0;

	// Create menu interface for assessment 1

	cout << " - Game Menu - " << endl;
	cout << " Press 1 for create Triangle" << endl;
	cout << " Press 2 for create Square" << endl;
	cout << " Press 3 for create Circle" << endl;
	cout << " Press 4 for create Trapezium" << endl;
	cin >> inputkey;

	return inputkey;
}
*/

Game::Game()
{
	cout << "Game Initialised!" << endl;

	Graphics = nullptr;
	bIsGameOver = false;
}

Game::~Game()
{
	// since nothing else is storing graphics memory
	// this will destroy the graphics from memory
	Graphics = nullptr;

	cout << "Game Over..." << endl;
}

void Game::Run()
{

	if (!bIsGameOver) {

		
		// create a shader
		Graphics->CreateShader({
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

		// Set/create texture image 
		//Graphics->CreateTexture("Game/Textures/ConcreteFloor.jpg");

		// create VAOs
		Graphics->CreateVAO(GeometricShapes::Square);
		Graphics->CreateVAO(GeometricShapes::Circle);
		Graphics->CreateVAO(GeometricShapes::Triangle);		
		Graphics->CreateVAO(GeometricShapes::Heart);

		
		

		
			
		

		

		
	}

	// if the game is not over run the loop
	while (!bIsGameOver) {
		// make sure we process what the user has done
		ProcessInput();
		// apply the logic base on the inputs and the game logic
		Update();
		// render the screen based on the 2 functions above
		Draw();
	}

	CloseGame();
}

void Game::ProcessInput()
{
	// TODO: Handle Input
	SDL_Event PollEvent;

	while (SDL_PollEvent(&PollEvent)) {
		
		switch (PollEvent.type) {
		case SDL_QUIT: 
			bIsGameOver = true;
			break;
		default:
			break;
		}
	}
}

void Game::Update()
{
	// TODO: Handle Update
}

void Game::Draw()
{
	// TODO: Draw graphics to the screen
	Graphics->Draw();
}

void Game::CloseGame()
{
	// TODO: Clean up code
}