#include "KSEngine/Graphics/GraphicsEngine.h"
#include "KSEngine/Graphics/VertexArrayObject.h"
#include "KSEngine/Graphics/ShaderProgram.h"
#include "KSEngine/Graphics/Texture.h"

#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
	bWireframeMode = false;
}

GraphicsEngine::~GraphicsEngine()
{
	// remove textures from memory
	TextureStack.clear();

	// this will handle deleting the SDL window from memory
	SDL_DestroyWindow(SdlWindow);
	// destroy the GL context for SDL
	SDL_GL_DeleteContext(SdlGLContext);
	// close the SDL framework
	SDL_Quit();

	cout << "Destroy Graphics Engine..." << endl;
}

bool GraphicsEngine::InitGE(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	// make sure SDL initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL failed: " << SDL_GetError() << endl;
		return false;
	}

	// Use OpenGL 4.6 compatability and set default attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // OpenGL version <"4"> .6 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); // OpenGL version  4 <".6">
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
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsEngine::Draw()
{
	ClearGraphics();

	HandleWireframeMode(false);

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
	}

	PresentGraphics();
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}

void GraphicsEngine::CreateVAO(GeometricShapes Shape)
{
	// create a new VAO as a shared pointer
	VAOPtr NewVAO = make_shared<VAO>(Shape);
	// add it to the stack
	VAOs.push_back(NewVAO);
}

void GraphicsEngine::CreateShader(VFShaderParams ShaderFilePaths)
{
	// create a new shader class
	ShaderPtr NewShader = make_shared<ShaderProgram>();

	// initialise the shader into openGL
	NewShader->InitVFShader(ShaderFilePaths);

	// add the shader to our graphics engine
	Shader = NewShader;
}

TexturePtr GraphicsEngine::CreateTexture(const char* FilePath)
{
	TexturePtr NewTexture = nullptr;

	// Run through all the textures and check if one with same path exists
	for (TexturePtr TestTexture : TextureStack) {
		// if we find a texture with the same file path
		if (TestTexture->GetFilePath() == FilePath) {
			// pass in the already created texture
			NewTexture = TestTexture;
			cout << "Texture found! Assigning current texture." << endl;
			break;
		}
	}

	// if there is no texture alredy in existance
	if (NewTexture == nullptr) {
		cout << "Creating a new texture..." << endl;

		// create a new texture as a shared_ptr
		NewTexture = make_shared<Texture>();

		// if the file was found, assign it to the texture stack
		if (NewTexture->CreateTextureFromFilePath(FilePath)) {
			cout << "Texture " << NewTexture->GetID() << " creation success! Adding to Texture Stack." << endl;

			// ass the texture to the texture stack
			TextureStack.push_back(NewTexture);
		}
	}

	return NewTexture;
}

void GraphicsEngine::HandleWireframeMode(bool bShowWireframeMode)
{
	// if wireframe mode is set, change it, and visa versa
	if (bShowWireframeMode != bWireframeMode) {
		bWireframeMode = bShowWireframeMode;

		// change how OpenGL renders between the vertices
		if (bWireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		cout << "Wireframe Mode Updated..." << endl;
	}
}