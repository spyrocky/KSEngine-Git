#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/Input.h"
#include "KSEngine/Graphics/Camera.h"

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
            L"Game/Shaders/TextureShader/TextureShader.svert",
            L"Game/Shaders/TextureShader/TextureShader.sfrag"
            });

        //create textures
        TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/ConcreteFloor.jpg");
        TexturePtr TGrid = Graphics->CreateTexture("Game/Textures/ColourGrid.jpg");

        //create meshes
        Poly = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TGrid });
        Poly2 = Graphics->CreateSimpleMeshShape(GeometricShapes::Cube, TextureShader, { TConcrete });

        

        //initial transformations for the meshes
        
        Poly->Transform.Location = Vector3(0.0f, 0.0f, 0.0f);
        Poly2->Transform.Location = Vector3(0.0f, 0.0f, 0.0f);
        

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

    CDirection CamDirection = Graphics->EngineDefaultCam->GetDirection();
    STCameraData CamData = Graphics->EngineDefaultCam->GetCameraData();

    //move camera foward
    if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
        CameraInput += CamDirection.Forward;
    }
    //move camera backward
    if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
        CameraInput += -CamDirection.Forward;
    }
    //move camera left
    if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
        CameraInput += -CamDirection.Right;
    }
    //move camera right
    if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
        CameraInput += CamDirection.Right;
    }
    //move camera up
    if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
        CameraInput += -CamDirection.Up;
    }//move camera down
    if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
        CameraInput += CamDirection.Up;
    }
  //Zoom in
    if (GameInput->IsKeyDown(SDL_SCANCODE_Z)) {
        CameraInput.x += CamData.FOV * 0.01f;

        cout << "FOV in: " << CameraInput.x << endl;
    }

    //Zoom Out
    if (GameInput->IsKeyDown(SDL_SCANCODE_C)) {
        CameraInput.x += -CamData.FOV * 0.01f;
        cout << "FOV out: " << CameraInput.x << endl;
    }

    //Zoom Reset to 70
    if (GameInput->IsKeyDown(SDL_SCANCODE_X)) {
        CameraInput += CamData.FOV;

        cout << "FOV reset: " << CameraInput.x << endl;
    }
   

    CameraInput *= 3.0f * GetFDeltaTime();

  

    Vector3 NewLocation = Graphics->EngineDefaultCam->GetTransforms().Location += CameraInput;
    Graphics->EngineDefaultCam->Translate(NewLocation);

    if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
        Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta * GetFDeltaTime() * 25.0f);
        Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta * GetFDeltaTime() * 25.0f);
    }

   
    

    //Test  mouse inputs
    /*if (GameInput->IsMouseButtonDown(MouseButtons::LEFT)) {
        cout << "Left Mouse button down..." << endl;
    }
    else {
        cout << "Left Mouse button up..." << endl;
    }
    

    cout << "x: " << GameInput->MouseX << " - y: " << GameInput->MouseY << endl;
        

    cout << GameInput->ScrollDelta << endl;*/


}

void Game::Draw()
{
    Graphics->Draw();
}

void Game::CloseGame()
{
    delete GameInput;
}