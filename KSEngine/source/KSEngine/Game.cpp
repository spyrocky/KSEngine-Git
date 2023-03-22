#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/Input.h"

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

Game::Game()
{
    cout << "Game Initialized" << endl;

    Graphics = nullptr;
    bIsGameOver = false;
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
        //create an input object to detect input
        GameInput = new Input();

        //create a shader
        ShaderPtr TextureShader = Graphics->CreateShader({
            L"Game/Shader/TextureShader/TextureShader.svert",
            L"Game/Shader/TextureShader/TextureShader.sfrag"
            });

        //create textures
        TexturePtr TBlueTiles = Graphics->CreateTexture("Game/Textures/BlueTiles.jpg");
        TexturePtr TGreenMosaic = Graphics->CreateTexture("Game/Textures/GreenMosaic.jpg");

        //create meshes
        Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TGreenMosaic });
        Poly2 = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TBlueTiles });

        MeshPtr test = Graphics->CreateSimpleMeshShape(GeometricShapes::Polygon, TextureShader, { TBlueTiles });

        test->Transform.Rotation.y = 60.0f;

        //initial transformations for the meshes
        Poly->Transform.Location.x = 1.0f;

        Poly2->Transform.Location.x = -1.0f;
        //Tri->Transform.Location.x = -0.5f;

        //Poly->Transform.Rotation.z = 45.0f;

        //Poly->Transform.Scale = Vector3(0.5f);
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
    //run the input detection for our game input
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

    Poly2->Transform.Rotation.z -= 50.0f * GetFDeltaTime();
    Poly2->Transform.Rotation.x -= 50.0f * GetFDeltaTime();
    Poly2->Transform.Rotation.y -= 50.0f * GetFDeltaTime();

    Vector3 CameraInput = Vector3(0.0f);
    Vector3 CameraRotate = Vector3(0.0f);

    //move camera foward
    if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
        CameraInput.z = 1.0f;
    }
    //move camera backward
    if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
        CameraInput.z = -1.0f;
    }
    //move camera left
    if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
        CameraInput.x = 1.0f;
    }
    //move camera right
    if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
        CameraInput.x = -1.0f;
    }
    //move camera up
    if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
        CameraInput.y = -1.0f;
    }
    //rotate camera left
    //if (GameInput->IsKeyDown(SDL_SCANCODE_LEFT)) {
    //    CameraInput.y = 1.0f;
    //}
    ////rotate camera right
    //if (GameInput->IsKeyDown(SDL_SCANCODE_RIGHT))

    CameraInput *= 1.0f * GetFDeltaTime();

    Graphics->EngineDefaultCam += CameraInput;
}

void Game::Draw()
{
    Graphics->Draw();
}

void Game::CloseGame()
{
    delete GameInput;
}