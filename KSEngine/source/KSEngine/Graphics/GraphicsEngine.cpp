#include "KSEngine/Graphics/GraphicsEngine.h"
#include "GL/glew.h"
#include "KSEngine/Graphics/Model.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "KSEngine/Graphics/Texture.h"
#include "KSEngine/Graphics/Camera.h"
#include "KSEngine/Graphics/Material.h"
#include "KSEngine/Collisions/Collosion.h"

using namespace std;

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
	bWireframeMode = false;
	//initialize camera -2.0 back on z axis
	EngineDefaultCam = make_shared<Camera>();
}

GraphicsEngine::~GraphicsEngine()
{
	//clear the model stack
	ModelStack.clear();

	//clear shader
	Shader = nullptr;

	//remove textures from memory
	TextureStack.clear();		//clear() empties the array/vector

	//this will handle deleting the sdl window from memory
	SDL_DestroyWindow(SdlWindow);
	//destroy the GL Context for sdl
	SDL_GL_DeleteContext(SdlGLContext);
	//close the sdl window
	SDL_Quit();

	cout << "Destroyed Graphics Engine..." << endl;
}

bool GraphicsEngine::InitGE(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	//make sure sdl initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL Failed: " << SDL_GetError() << endl;
		return false;
	}

	//Use OpenGL 4.6 compatibility and set default attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	//Set the fullscreen flag
	int FullscreenFlag = 0;

	if (bFullscreen) {
		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	}
	else {
		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}

	//create the sdl2 window
	SdlWindow = SDL_CreateWindow(
		WTitle,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WWidth, WHeight,
		FullscreenFlag
	);

	//if the window fails
	if (SdlWindow == nullptr) {
		cout << "SDL Window Failed: " << SDL_GetError() << endl;
	}

	//add all the gl attributes to the window
	SdlGLContext = SDL_GL_CreateContext(SdlWindow);

	if (SdlGLContext == NULL) {
		cout << "SDL GL Context Failed: " << SDL_GetError() << endl;
		return false;
	}

	//to make glew work we need to mark experimental true
	glewExperimental = GL_TRUE;

	const GLenum InitGLEW = glewInit();

	if (InitGLEW != GLEW_OK) {
		cout << "Glew Failed: " << glewGetErrorString(InitGLEW) << endl;
		return false;
	}

	//enable 3D depth
	glEnable(GL_DEPTH_TEST);

	// Create Default Engine texture
	DefaultEngineTexture = CreateTexture("Game/Textures/DefaultTexture.png");

	//create Default engine material
	//add material when create auto assign the default texture
	DefaultEngineMaterial = make_shared<Material>();

	return true;
}

void GraphicsEngine::PresentGraphics()
{
	//present the new graphics using opengl
	SDL_GL_SwapWindow(SdlWindow);
}

void GraphicsEngine::ClearGraphics()
{
	//set the background color
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::Draw()
{


	HandleWireframeMode(false);
	 
	/* //---- Test draw box colision ----
	//create the box colision
                                         // colision posision x,y,z , //           ,// colision size scale x,y,z
	static BoxCollisionPtr col = make_shared<BoxCollision>(Vector3(-3.0f, 0.0f, 0.0f), Vector3(0.0f,0.0f, 0.0f), Vector3(3.0f, 3.0f, 3.0f));
							//green color collision box
	col->DebugDraw(Vector3(0.0f,255.0f,0.0f));
	*/


	//run through each Model and call its draw method
	for (ModelPtr LModel : ModelStack) {
		LModel->Draw();
	}


}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}

ModelPtr GraphicsEngine::CreateSimpleModelShape(GeometricShapes Shape, ShaderPtr MeshShader)
{
	//initialize a new model class
	ModelPtr NewModel = make_shared<Model>();

	//make sure it worked
	if (!NewModel->CreateSimpleMesh(Shape, MeshShader)) {
		return nullptr;
	}

	//add mesh into the stack of meshses to be rendered
	ModelStack.push_back(NewModel);

	//return the new mesh
	return NewModel;
}

ModelPtr GraphicsEngine::ImportModel(const char* FilePath, ShaderPtr Shader)
{
	ModelPtr NewModel = make_shared<Model>();

	if (!NewModel->ImportMeshFromFile(FilePath, Shader)) {
		return nullptr;
	}

	return NewModel;
}

ShaderPtr GraphicsEngine::CreateShader(VFShaderParams ShaderFilePaths)
{
	//create a new shader class
	ShaderPtr NewShader = make_shared<ShaderProgram>();
	//initialize the shader into openGL using the file paths
	NewShader->InitVFShader(ShaderFilePaths);
	//add the shader to our graphics engine
	Shader = NewShader;

	return NewShader;
}

TexturePtr GraphicsEngine::CreateTexture(const char* FilePath)
{
	TexturePtr NewTexture = nullptr;

	//Run through all the texture and check if one with the same path exists
	for (TexturePtr TestTexture : TextureStack) {
		//if we find a texture with the same file path
		if (TestTexture->GetFilePath() == FilePath) {
			//pass in the already created texture
			NewTexture = TestTexture;
			cout << "Texture found: Assigning current texture." << endl;
			break;
		}
	}

	//if there is no texture already existing
	if (NewTexture == nullptr) {
		cout << "Creating a new texture..." << endl;

		//create a new texture as a shared ptr
		NewTexture = make_shared<Texture>();

		//if the file was found assign it to the texture stack
		if (NewTexture->CreateTextureFromFilePath(FilePath)) {
			cout << "Texture " << NewTexture->GetID() << " creation successful! Adding to Texture Stack." << endl;

			//add the texture to the texture stack
			TextureStack.push_back(NewTexture);
		}
	}

	return NewTexture;
}

void GraphicsEngine::ApplyScreenTransformations(ShaderPtr Shader)
{
	//the angle of the camera planes  - zoom
	float FOV = EngineDefaultCam->GetCameraData().FOV;
	//position of t	he camera/view space
	Vector3 ViewPosition = EngineDefaultCam->GetTransforms().Location;
	//find the size of the screen and calculate the aspect ratio
	int WWidth, WHeight = 0;
	//use SDL to get the size of the window
	SDL_GetWindowSize(SdlWindow, &WWidth, &WHeight);
	//calculate the aspect ratio from the window size
	float AR = static_cast<float>(WWidth) / static_cast<float>(max(WHeight, 1));

	//create the default coordinates for the projection and view
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//update the coordinates for 3D
	view = EngineDefaultCam->GetViewMatrix();
	//create the perspective view to allow us to see in 3D
	//also adjusting the near and far clip
	projection = glm::perspective(glm::radians(FOV), AR, EngineDefaultCam->GetCameraData().NearClip, EngineDefaultCam->GetCameraData().FarClip);

	Shader->SetMat4("view", view);
	Shader->SetMat4("projection", projection);
}

void GraphicsEngine::RemoveModel(ModelPtr ModelToRemove)
{
	// Looking for the model in the model stack vector array
	// we look to the old vector and if we find the value then we assign the correct index
	// this will equal .end() if it doesn't exist the stack 
	ModelPtrStack::iterator ModelIndex = find(ModelStack.begin(), ModelStack.end(), ModelToRemove);

	// if it's not in the array then stop the function
	if (ModelIndex == ModelStack.end()) {
		return;
	}
	//use thw iterator/index to erase the object
	ModelStack.erase(ModelIndex);
}

void GraphicsEngine::HandleWireframeMode(bool bShowWireframeMode)
{
	// if wireframe mode is set, change it and vice versa
	if (bShowWireframeMode != bWireframeMode) {
		bWireframeMode = bShowWireframeMode;

		//change how openGL renders between vertices
		if (bWireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		cout << "Wireframe Mode Updated..." << endl;
	}}