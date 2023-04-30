#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/Input.h"
#include "KSEngine/Graphics/Camera.h"
#include "KSEngine/Graphics/Material.h"
#include "KSEngine/Collisions/Collosion.h"


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

        // create the materials
        MaterialPtr MConcrete = make_shared<Material>();
        MaterialPtr MGrid = make_shared<Material>();

        //Assign base coloe using the texture
        MConcrete->BaseColour.TextureV3 = TConcrete;
        MGrid->BaseColour.TextureV3 = TGrid;

        //create meshes / 3 Primitives
        Model = Graphics->ImportModel("Game/Models/Primitives/Cube.fbx", TextureShader);
        Model2 = Graphics->ImportModel("Game/Models/Primitives/Sphere.fbx", TextureShader);      

        //set material of the model
        Model->SetMaterialBySlot(0, MGrid);
        Model2->SetMaterialBySlot(0, MConcrete);

        Model2->GetMaterialBySlot(0)->EmissiveColour.MultiplierV3 = Vector3(1.0f, 0.0f,0.0f);

        // transformations for the model location        
        Model->Transform.Location = Vector3(0.0f,0.0f, 0.0f);
        Model2->Transform.Location = Vector3(20.0f, 0.0f, 0.0f);
      

        //import custom meshes
        Wall = Graphics->ImportModel("Game/Models/damaged-wall/SM_Wall_Damaged.obj", TextureShader);
        
        //transform the wall
        Wall->Transform.Scale = Vector3(0.05f);
        Wall->Transform.Rotation.y = 90.0f;
        Wall->Transform.Location = Vector3(50.0f, -2.0f, 0.0f);

        //create the texture
        TexturePtr TWall = Graphics->CreateTexture("Game/Models/damaged-wall/textures/T_Wall_Damaged_2x1_A_BC.png");
        MaterialPtr MWall = make_shared<Material>();
        MWall->BaseColour.TextureV3 = TWall;


        //apply the material
        Wall->SetMaterialBySlot(1, MWall);

        //collision wireframe box collision size x,y,z   
        Model->AddCollisionToModel(Vector3(2.5f));
        Model2->AddCollisionToModel(Vector3(4.0f));
        Wall->AddCollisionToModel(Vector3(2.5f, 4.0f, 10.0f), Vector3(0.0f, 2.0f, 0.0f));


        //---------------------------------- Game obj -------------------------------------

        Trex = Graphics->ImportModel("Game/Models/Dinosaur/FBX/Trex.fbx", TextureShader);
        Tree = Graphics->ImportModel("Game/Models/Tree/tree-oval.obj", TextureShader);
        Coin = Graphics->ImportModel("Game/Models/coin/OBJ/SimpleCoin.obj", TextureShader);       

        Tree->Transform.Scale = Vector3(0.05f);
        Tree->Transform.Rotation.y = 90.0f;
        Tree->Transform.Location = Vector3(2.0f, -2.0f, 0.0f);

        Coin->Transform.Scale = Vector3(0.05f);
        Coin->Transform.Rotation.y = 90.0f;
        Coin->Transform.Location = Vector3(5.0f, -2.0f, 0.0f);  
       

        TexturePtr TTree = Graphics->CreateTexture("Game/Textures/GreenMosaic.jpg");
        //create a material
        MaterialPtr MTree = make_shared<Material>();
        MTree->BaseColour.TextureV3 = TTree;

        TexturePtr TCoin = Graphics->CreateTexture("Game/Textures/Spongebob.jpg");
        //create a material
        MaterialPtr MCoin = make_shared<Material>();
        MCoin->BaseColour.TextureV3 = TCoin;

        TexturePtr TTrex = Graphics->CreateTexture("Game/Textures/GreenMosaic.jpg");
        //create a material
        MaterialPtr MTrex = make_shared<Material>();
        MTrex->BaseColour.TextureV3 = TTrex;

        
        //apply the material
       Tree->SetMaterialBySlot(1, MTree);
       Coin->SetMaterialBySlot(1, MCoin);
       Trex->SetMaterialBySlot(1, MTrex);

       Tree->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Coin->AddCollisionToModel(Vector3(2.5f, 2.5f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Trex->AddCollisionToModel(Vector3(5.0f, 5.0f, 5.0f), Vector3(0.0f, 2.0f, 0.0f));

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

    Vector3 CameraInput = Vector3(0.0f);

    CDirection CamDirection = Graphics->EngineDefaultCam->GetDirection();

    Graphics->EngineDefaultCam->GetCameraData().FarClip;
   /* Vector3 CameraInput = Vector3(0.0f);
    Vector3 CameraRotate = Vector3(0.0f);
    CDirection CamDirection = Graphics->EngineDefaultCam->GetDirection();
    STCameraData CamData = Graphics->EngineDefaultCam->GetCameraData();
    */


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

    /*
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
    }*/

    //move the camera based on input
    Graphics->EngineDefaultCam->AddMovementInput(CameraInput);

    if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
        Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta);
        Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta);
        GameInput->ShowCursor(false);
    }
    else {
        GameInput->ShowCursor(true);
    }
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

    //just move toward slowly for the obstacle such as tree and coin
    Wall->Transform.Location.x += -1.0f * GetFDeltaTime();


    //Collision check and collectable item such as coin
    if (Wall != nullptr) {
        Wall->Transform.Rotation.y += 50.0f * GetFDeltaTime();
    }
    


    //TODO:Handle Logic
    if(Model != nullptr){
    Model->Transform.Rotation.z += 50.0f * GetFDeltaTime();
    Model->Transform.Rotation.x += 50.0f * GetFDeltaTime();
    Model->Transform.Rotation.y += 50.0f * GetFDeltaTime();
    }

    if (Model != nullptr) {
        Model2->Transform.Rotation.z -= 50.0f * GetFDeltaTime();
        Model2->Transform.Rotation.x -= 50.0f * GetFDeltaTime();
        Model2->Transform.Rotation.y -= 50.0f * GetFDeltaTime();
    }

    if (Wall != nullptr) {
        Wall->Transform.Rotation.y += 50.0f * GetFDeltaTime();
    }

        
    Graphics->EngineDefaultCam->Update();

    //Do collision stuff
    CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();
    
    //check collider of 2 object

    // if we run into the wall then remove it from game
    if (Wall != nullptr && CamCol->IsOverlapping(*Wall->GetCollision())) {      
        //cout << "It Collider !" << endl;
        //Remove it from the game
        RemoveModelFromGame(Wall);
    }
    

}

void Game::Draw()
{
    Graphics->ClearGraphics();
  // TODO : Draw graphic on screen
    Graphics->Draw();

    //debug draw the camera collision
    CollisionPtr CamCol =  Graphics->EngineDefaultCam->GetCameraCollision();
    CamCol->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));

    //check collider of 2 object
   /* if (Tree != nullptr && CamCol->IsOverlapping(*Tree->GetCollision())) {
        //if collider then draw green collider if colliding with Tree
        CamCol->DebugDraw(Vector3(0.0f,255.0f,0.0f));
        cout << "It Collider !" << endl;
    }
    else {
        //if not Draw red collider  with Tree
        CamCol->DebugDraw(Vector3(255.0f, 0.0f, 0.0f));
    }*/

    Model->GetCollision()->DebugDraw(Vector3(255.0f, 0.0f, 0.0f));
    Model2->GetCollision()->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));
    Wall->GetCollision()->DebugDraw(Vector3(0.0f, 0.0f, 255.0f));

    Graphics->PresentGraphics();
}

void Game::CloseGame()
{
    delete GameInput;
}

TexturePtr Game::GetDefaultGEngineTexture()
{
    return Graphics->DefaultEngineTexture;
}

MaterialPtr Game::GetDefaultGEngineMaterial()
{
    
    return Graphics->DefaultEngineMaterial;
}

void Game::RemoveModelFromGame(ModelPtr& ModelToRemove)
{
    //remove from the graphics engine 
    Graphics->RemoveModel(ModelToRemove);
    //Change the reference to nullptr and remove from game
    ModelToRemove = nullptr;
}

