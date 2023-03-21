#include "KSEngine/Graphics/GraphicsEngine.h"
#include "GL/glew.h"
#include "KSEngine/Graphics/Mesh.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "KSEngine/Graphics/Texture.h"
#include "KSEngine/Math/Transformations.h"

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
	bWireframeMode = false;
	EngineDefaultCam = Vector3(0.0f, 0.0f, -2.0f);
}

GraphicsEngine::~GraphicsEngine()
{
	//clear the mesh stack
	MeshStack.clear();

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
	// make sure SDL initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL failed: " << SDL_GetError() << endl;
		return false;
	}

	// Use OpenGL 4.6 compatability and set default attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL version <"4"> .6 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); // OpenGL version  4 <".6">
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY); // Using default profile
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // Transparency level
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // How many bits of allocation awarded to "RED" colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); // How many bits of allocation awarded to "GREEN" colour
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); // How many bits of allocation awarded to "BLUE" colour

	// Set the fullscreen flag
	int FullScreenFlag = 0;

	if (bFullscreen) {
		FullScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	}
	else {
		FullScreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}

	// create the SDL2 window
	SdlWindow = SDL_CreateWindow(
		WTitle, // title of the window
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // location of the window
		WWidth, WHeight, // width and height of the window
		FullScreenFlag
	);

	// if the window fails then error log
	if (SdlWindow == nullptr) {
		cout << "SDL window failed: " << SDL_GetError() << endl;
		return false;
	}

	// add all the gl attributes to the window
	SdlGLContext = SDL_GL_CreateContext(SdlWindow);

	if (SdlGLContext == NULL) {
		cout << "SDL GL Context failed: " << SDL_GetError() << endl;
		return false;
	}

	// To make glew work we need to mark experimental true
	glewExperimental = GL_TRUE;

	const GLenum InitGLEW = glewInit();

	if (InitGLEW != GLEW_OK) {
		cout << "GLEW failed: " << glewGetErrorString(InitGLEW) << endl;
		return false;
	}
	//enable 3d depth
	glEnable(GL_DEPTH_TEST);

	return true;
}

void GraphicsEngine::PresentGraphics()
{
	// present the new graphics using opengl
	SDL_GL_SwapWindow(SdlWindow);
}

void GraphicsEngine::ClearGraphics()
{
	// set the background colour
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsEngine::Draw()
{
	ClearGraphics();

	HandleWireframeMode(false);

<<<<<<< Updated upstream
	unInt index = 0;
	// TODO: Add anything that renders between these two functions
	for (VAOPtr VAO : VAOs) {
		Shader->RunShader();

		// move the object
		glm::mat4 transform = glm::mat4(1.0f);

		if (index == 0) {
			// move in the x, y, or z, direction based on the amount added
			transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
			// radians is rotation amount
			// vec3 is the direction to rotate in
			transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			// x and y will work for out 2D shapes
			// z must be larger than 0 or you wont see the object (1 is default)
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		}
		else if (index == 1) {
			transform = glm::translate(transform, glm::vec3(-0.5f, 0.0f, 0.0f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		}
		else if (index == 2) {
			transform = glm::translate(transform, glm::vec3(0.0f, 0.5f, 0.0f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		}
		else if (index == 3) {
			transform = glm::translate(transform, glm::vec3(0.0f, -0.5f, 0.0f));
			transform = glm::scale(transform, glm::vec3(0.25f, 0.25f, 1.0f));
		}

		Shader->SetMat4("transform", transform);
		// draw each VAO
		VAO->Draw();

		index++;
=======
	//run through each mesh and call its draw method
	for (MeshPtr LMesh : MeshStack) {
		LMesh->Draw();
>>>>>>> Stashed changes
	}

	PresentGraphics();
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}

MeshPtr GraphicsEngine::CreateSimpleMeshShape(GeometricShapes Shape, ShaderPtr MeshShader, TexturePtrStack MeshTextures)
{
	//initialize a new mesh class
	MeshPtr NewMesh = make_shared<Mesh>();

	//make sure it worked
	if (!NewMesh->CreateSimpleShape(Shape, MeshShader, MeshTextures))
		return nullptr;

	//add mesh into the stack of meshses to be rendered
	MeshStack.push_back(NewMesh);

	//return the new mesh
	return NewMesh;
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

void GraphicsEngine::ApplyScreenTransformation(ShaderPtr Shader)
{
	// the andgle of camera plane - basically your zoom
	float FOV = 70.0f;
	// position of the camera / view space 
	Vector3 ViewPosition = EngineDefaultCam;
	// find the size of the screen and calculate the aspect ratio
	int WWidth, WHeight = 0;
	//use sdl to get the size of window
	SDL_GetWindowSize(SdlWindow, &WWidth, &WHeight);
	//calculate the aspect ratio from window size
	float AR = static_cast<float>(WWidth) / static_cast<float>(max(WHeight, 1));
	//create the default coordinate of the projection and view
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//update the coordinates for 3d
	view = glm::translate(view, ViewPosition);
	//crete the perspective view to allow us to see in 3d
	// also adjust the near and far clip
	projection = glm::perspective(glm::radians(FOV), AR, 0.01f,  1000.0f);

	Shader->SetMat4("view", view);
	Shader->SetMat4("projection", projection);
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
	}
}
