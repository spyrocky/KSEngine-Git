#include "KSEngine/Game.h"
#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "KSEngine/Graphics/Texture.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Input.h"
#include "KSEngine/Graphics/Camera.h"
#include "KSEngine/Graphics/Material.h"
#include "KSEngine/Collisions/Collosion.h"
#include "KSEngine/Graphics/Text.h"
#include "SDL2/SDL.h"


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


        //---------------------------------- Game obj -------------------------------------
        Trex = Graphics->ImportModel("Game/Models/dino-low-poly/source/Dino_LowPoly.fbx", TextureShader);
        
        //import obj custom mesh to game (obstacles)
        //test 3d model
        Tree = Graphics->ImportModel("Game/Models/lowpolytree/source/PP_Tree_winding_01.fbx", TextureShader);
        Tree2 = Graphics->ImportModel("Game/Models/lowpolytree/source/PP_Tree_winding_01.fbx", TextureShader);
        Tree3 = Graphics->ImportModel("Game/Models/lowpolytree/source/PP_Tree_winding_01.fbx", TextureShader);
        Tree4 = Graphics->ImportModel("Game/Models/lowpolytree/source/PP_Tree_winding_01.fbx", TextureShader);
        Tree5 = Graphics->ImportModel("Game/Models/lowpolytree/source/PP_Tree_winding_01.fbx", TextureShader);



        // items collectable
        //Test 3d model
        Coin = Graphics->ImportModel("Game/Models/stylized-coin/source/Coin.fbx", TextureShader);
        Coin2 = Graphics->ImportModel("Game/Models/stylized-coin/source/Coin.fbx", TextureShader);
        Coin3 = Graphics->ImportModel("Game/Models/stylized-coin/source/Coin.fbx", TextureShader);

       

        //Transform all
        Trex->Transform.Scale = Vector3(0.5f);
        Trex->Transform.Rotation.x = 200.0f;
        Trex->Transform.Location = Vector3(0.0f, 0.0f, 0.0f);

        Tree->Transform.Scale = Vector3(0.6f);      
        Tree->Transform.Location = Vector3(2.0f, 0.0f, 1.0f);

        Tree2->Transform.Scale = Vector3(0.5f);
        Tree2->Transform.Location = Vector3(10.0f, 0.0f, -1.0f);

        Tree3->Transform.Scale = Vector3(0.8f);
        Tree3->Transform.Location = Vector3(20.0f, 0.0f, 3.0f);

        Tree4->Transform.Scale = Vector3(0.4f);
        Tree4->Transform.Location = Vector3(30.0f, 0.0f, 2.0f);

        Tree5->Transform.Scale = Vector3(0.7f);
        Tree5->Transform.Location = Vector3(40.0f, 0.0f, -1.0f);


        Coin->Transform.Scale = Vector3(3.0f);
        Coin->Transform.Rotation.y = 90.0f;
        Coin->Transform.Location = Vector3(5.0f, 3.0f, 0.0f);  

        Coin2->Transform.Scale = Vector3(3.0f);
        Coin2->Transform.Rotation.y = 90.0f;
        Coin2->Transform.Location = Vector3(25.0f, 0.0f, -1.0f);

        Coin3->Transform.Scale = Vector3(3.0f);
        Coin3->Transform.Rotation.y = 90.0f;
        Coin3->Transform.Location = Vector3(35.0f, 1.0f, 1.0f);
       

        //create texture
        TexturePtr TTrex = Graphics->CreateTexture("Game/Textures/Spongebob.jpg");
        TexturePtr TTree = Graphics->CreateTexture("Game/Textures/ColourGrid.jpg");
        TexturePtr TCoin = Graphics->CreateTexture("Game/Models/stylized-coin/textures/Coin2_BaseColor.jpg");

        //create a material
        MaterialPtr MTrex = make_shared<Material>();
        MTrex->BaseColour.TextureV3 = TTrex;

        MaterialPtr MTree = make_shared<Material>();
        MTree->BaseColour.TextureV3 = TTree;

        MaterialPtr MCoin = make_shared<Material>();
        MCoin->BaseColour.TextureV3 = TCoin;


        //apply the material
       Trex->SetMaterialBySlot(1, MTrex);
       Tree->SetMaterialBySlot(1, MTree);
       Tree2->SetMaterialBySlot(1, MTree);
       Tree3->SetMaterialBySlot(1, MTree);
       Tree4->SetMaterialBySlot(1, MTree);
       Tree5->SetMaterialBySlot(1, MTree);

       Coin->SetMaterialBySlot(1, MCoin);
       Coin2->SetMaterialBySlot(1, MCoin);
       Coin3->SetMaterialBySlot(1, MCoin);

     
       //light
       Tree5->GetMaterialBySlot(0)->EmissiveColour.MultiplierV3 = Vector3(0.5f, 0.0f, 0.0f);       
       Tree->GetMaterialBySlot(0)->EmissiveColour.MultiplierV3 = Vector3(0.5f, 0.5f, 0.0f);



       //Add collision box
       Trex->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Tree->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Tree2->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Tree3->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Tree4->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Tree5->AddCollisionToModel(Vector3(2.5f, 5.0f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));

       Coin->AddCollisionToModel(Vector3(2.5f, 2.5f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Coin2->AddCollisionToModel(Vector3(2.5f, 2.5f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
       Coin3->AddCollisionToModel(Vector3(2.5f, 2.5f, 2.5f), Vector3(0.0f, 2.0f, 0.0f));
      

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

    
    //Rotate collectable item such as coin
    if (Coin != nullptr) {
        Coin->Transform.Rotation.y += 50.0f * GetFDeltaTime(); //rotate coin
    }

    if (Coin2 != nullptr) {
      Coin2->Transform.Rotation.y += 50.0f * GetFDeltaTime(); //rotate coin
    }

    if (Coin3 != nullptr) {
        Coin3->Transform.Rotation.y += 50.0f * GetFDeltaTime(); //rotate coin
    }
    
    

    Graphics->EngineDefaultCam->Update();

    //Do collision stuff
    CollisionPtr CamCol = Graphics->EngineDefaultCam->GetCameraCollision();
    
    //check collider of 2 object

    // if we run into the coin then remove it from game
    if (Coin != nullptr && CamCol->IsOverlapping(*Coin->GetCollision())) {
        //cout << "It Collider !" << endl;
        //Remove it from the game
        RemoveModelFromGame(Coin);
    }

    if (Coin2 != nullptr && CamCol->IsOverlapping(*Coin2->GetCollision())) {
        RemoveModelFromGame(Coin2);
    }

    if (Coin3 != nullptr && CamCol->IsOverlapping(*Coin3->GetCollision())) {
        RemoveModelFromGame(Coin3);
    }
    

}

void Game::Draw()
{
    Graphics->ClearGraphics();
  // TODO : Draw graphic on screen
    Graphics->Draw();

    /*SDL_Color TextColour = {255};
      ScoreText = make_shared<Text>("Game/Fonts/BrunoAce-Regular.ttf", "Score: 0", 25, 25, TextColour);*/

    //debug draw the camera collision
    CollisionPtr CamCol =  Graphics->EngineDefaultCam->GetCameraCollision();    
    //CamCol->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));

    //Put Trex in camera to do FPS Game
    //if (Trex != nullptr) {
    //    Trex->Transform.Location = Vector3(2.0f, 0.0f, 1.0f);
    //}


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

    if (Coin != nullptr && CamCol->IsOverlapping(*Coin->GetCollision())) {
        CamCol->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));
    }
    else {
        //CamCol->DebugDraw(Vector3(255.0f, 0.0f, 0.0f));
    }

    if (Coin2 != nullptr && CamCol->IsOverlapping(*Coin2->GetCollision())) {
        CamCol->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));
    }
    else {
       // CamCol->DebugDraw(Vector3(255.0f, 0.0f, 0.0f));
    }

    if (Coin3 != nullptr && CamCol->IsOverlapping(*Coin3->GetCollision())) {
        CamCol->DebugDraw(Vector3(0.0f, 255.0f, 0.0f));
    }
    else {
        //CamCol->DebugDraw(Vector3(255.0f, 0.0f, 0.0f));
    }
    

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

