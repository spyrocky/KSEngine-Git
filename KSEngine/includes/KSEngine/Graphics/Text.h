#pragma once

struct SDL_Texture;
struct SDL_Surface;
struct SDL_Color;
struct SDL_Renderer;
struct SDL_Rect;

class Text {
public:
	Text(const char* FontFilePath, const char* String, int x, int y,SDL_Color* Colour,int DefaultSize = 24);
	~Text();

	void Draw();


private:
	SDL_Texture* FontTexture;
	SDL_Surface* FontSurface;

	SDL_Renderer* Renderer;

	SDL_Rect* FontInfo;
};