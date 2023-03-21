#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/input.h"


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

Game::Game()
{
    cout << "Game Initialized" << endl;

    Graphics = nullptr;
    bIsGameOver = false;

    GameInput = new Input();
}

Game::~Game()
{
    //since nothing else is storing the graphics in memory
    //this will destroy the graphics from the memory
    Graphics = nullptr;

    cout << "Game Over..." << endl;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
    //Load the window using graphics class
    Graphics = make_shared<GraphicsEngine>();

    //if the window fails to load then bIsGameOver = true
    if (!Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight)) {
        bIsGameOver = true;
    }

    Run();
}

void Game::Run()
{
	if (!bIsGameOver) {
		// create a shader
		Graphics->CreateShader({
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

		Graphics->CreateTexture("Game/Textures/ConcreteFloor.jpg");

		// create VAOs
		/*Graphics->CreateVAO(GeometricShapes::Square);
		Graphics->CreateVAO(GeometricShapes::Circle);
		Graphics->CreateVAO(GeometricShapes::Trapezium);
		Graphics->CreateVAO(GeometricShapes::Triangle);*/
	}

	// as long as the game is not over run the loop
	while (!bIsGameOver) {
		// make sure we process what the user has done
		ProcessInput();
		// apply the logic base on the inputs and the game logic
		Update();
		// render the screen based on the 2 functions above
		Draw();
	}

    if (!bIsGameOver) {

        //Create input class to detect input
        GameInput = new Input();

        //create a shader
        ShaderPtr TextureShader = Graphics->CreateShader({
            L"Game/Shader/TextureShader/TextureShader.svert",
            L"Game/Shader/TextureShader/TextureShader.sfrag"
            });

        //create textures
        TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/ConcreteFloor.jpg");
        TexturePtr TGride = Graphics->CreateTexture("Game/Textures/ColourGrid.jpg");

        //create meshes
        Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TConcrete });
        Poly2 = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TGride });


        MeshPtr test = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TConcrete });
  
        Poly->Transform.Location.x = 1.0f;
        Poly2->Transform.Location.x = -1.0f;
    }

    //as long as the game is not over
    while (!bIsGameOver) {
        //make sure we process the user input
        ProcessInput();

        //apply logic based on the inputs and game logic
        Update();

        //render the screen based on the functions above
        Draw();
    }

    CloseGame();
}

void Game::ProcessInput()
{

	// TODO: Handle Input
	SDL_Event PollEvent;

	//
	while (SDL_PollEvent(&PollEvent)) {
		//
		switch (PollEvent.type) {
		case SDL_QUIT: //
			bIsGameOver = true;
			break;
		default:
			break;
		}
	}

    //run the input detection for oue game input
    GameInput->ProcessInput();

}

void Game::Update()
{
    //Set delta time first always
    static double LastFrameTime = 0.0;
    //Set the current time since the game has passed
    double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
    //find the time difference between last and current frame
    double NewDeltaTime = CurrentFrameTime - LastFrameTime;
    //set delta time as seconds
    DeltaTime = NewDeltaTime / 1000.0;
    //update the last frame tiem for the next update
    LastFrameTime = CurrentFrameTime;


    //TODO:Handle Logic
    Poly->Transform.Rotation.z += 50.0f * GetFDeltaTime();
    Poly->Transform.Rotation.x += 50.0f * GetFDeltaTime();
    Poly->Transform.Rotation.y += 50.0f * GetFDeltaTime();

    Poly2->Transform.Rotation.z += 50.0f * GetFDeltaTime();
    Poly2->Transform.Rotation.x += 50.0f * GetFDeltaTime();
    Poly2->Transform.Rotation.y += 50.0f * GetFDeltaTime();

    Vector3 CameraInput = Vector3(0.0f);
    //move cam forward
    if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
        CameraInput.z = 1.0f;
    }
    //backward
    if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
        CameraInput.z = -1.0f;
    }

    //left
    if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
        CameraInput.x = 1.0f;
    }
    //right
    if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
        CameraInput.x = -1.0f;
    }

    //up
    if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
        CameraInput.y = -1.0f;
    }
    //dow
    if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
        CameraInput.y = 1.0f;
    }

    //speed for moveing cam
    CameraInput *= 3.0f * GetDeltaTime();

    Graphics->EngineDefaultCam += CameraInput;

    
       
    
}

void Game::Draw()
{
    Graphics->Draw();
}

void Game::CloseGame()
{
    // TODO: Clean up code
    delete GameInput;
}