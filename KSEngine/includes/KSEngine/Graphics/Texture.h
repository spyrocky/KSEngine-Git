#pragma once
#include "KSEngine/CoreMinimal.h"

// hold the image data for conversion to the texture file
struct ImportImageData {
	int w = 0;
	int h = 0;
	int channels = 0;
};

class Texture {
public:
	Texture();
	~Texture();

	//Create texture file from imported image path
	bool CreateTextureFromFilePath(const char* FilePath);

	//return the openGL texture ID
	unInt GetID() const { return TextureID; }

	//bind the texture as the current texture in openGL
	void BindTexture();

	//assigns the relevant data to the current loaded texture
	void ActivateTexture(unInt Index);

	//clear the texture in openGL
	static void ResetTexture();

	//return the file path
	const char* GetFilePath() const { return FilePath; }

private:
	//hold the ID for the texture in OpenGL
	unInt TextureID;
	//hold the filepath to avoid duplicates
	const char* FilePath;
};