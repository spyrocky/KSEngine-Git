#pragma once
#include "KSEngine/CoreMinimal.h"

// hold the image data for conversion to the texture file
struct ImportImageData {
	int w = 0; // image width
	int h = 0; // image height
	int channels = 0; // how many channels are in an image (R, G, B, A/ BW, A)
};

class Texture {
public:
	Texture();
	~Texture();

	// create texture file from imported image path
	bool CreateTextureFromFilePath(const char* FilePath);

	// return the OpenGL texture ID
	unInt GetID() const { return TextureID; }

	// bind the texture as the current texture in OpenGL
	void BindTexture();

	// assign the relevant data to the current loaded texture
	void ActivateTexture();

	// clear the texture in OpenGl
	static void ResetTexture();

	// return the file path
	const char* GetFilePath() { return FilePath; }

private:
	// hold the ID for the texture in OpenGL
	unInt TextureID;
	// hold the filepath to avoid duplicates
	const char* FilePath;
};