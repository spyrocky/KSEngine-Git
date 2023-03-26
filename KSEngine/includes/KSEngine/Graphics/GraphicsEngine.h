#pragma once
#include "KSEngine/CoreMinimal.h"
#include "SDL2/SDL.h"
#include "KSEngine/Math/Transformations.h"

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	//create the window
	bool InitGE(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);

	//show the newest frame graphics
	void PresentGraphics();
	//clear the old frame graphics
	void ClearGraphics();
	//handle the rendering of graphics
	void Draw();

	//return the sdl window
	SDL_Window* GetWindow() const;

	//add a new VAO to the VAOStack using a shape define
	MeshPtr CreateSimpleMeshShape(GeometricShapes Shape, ShaderPtr MeshShader, TexturePtrStack MeshTextures);

	//add new shader
	//@param 1 - Vertex Shader
	//@param 2 - Fragment Shader
	ShaderPtr CreateShader(VFShaderParams ShaderFilePaths);

	//create a texture and add it to the texture stack
	//avoid duplicates
	TexturePtr CreateTexture(const char* FilePath);

	//create the 3D space for the model to relate itself to
	//screen and camera coordinates
	void ApplyScreenTransformations(ShaderPtr Shader);

private:
	//this will hold the window
	SDL_Window* SdlWindow;
	//this will allow openGL to work in sdl
	SDL_GLContext SdlGLContext;
	//handle wireframe mode
	void HandleWireframeMode(bool bShowWireframeMode);
	//set wireframe
	bool bWireframeMode;

	//single shader
	ShaderPtr Shader;
	//store a vector of textures
	TexturePtrStack TextureStack;
	//store all meshes in the game
	MeshPtrStack MeshStack;

public:
	//default camera 
	CameraPtr EngineDefaultCam;
};