#include "KSEngine/Graphics/Text.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "KSEngine/Game.h"
#include "KSEngine/Graphics/GraphicsEngine.h"

Text::Text(const char* FontFilePath, const char* String,int x,int y, SDL_Color* Colour, int DefaultSize)
{
	//install font
	TTF_Font* Font = TTF_OpenFont(FontFilePath, DefaultSize);

	FontSurface = TTF_RenderText_Solid(Font,String,*Colour);

	Renderer = Game::GetGameInstance().GetGraphicsEngine()->UIRenderer;

	FontTexture = SDL_CreateTextureFromSurface(Renderer, FontSurface);

	FontInfo = new SDL_Rect();
	FontInfo->x = x;
	FontInfo->y = y;
	FontInfo->w = FontSurface->w;
	FontInfo->h = FontSurface->h;

}

Text::~Text()
{
	SDL_DestroyTexture(FontTexture);
	SDL_FreeSurface(FontSurface);

	delete FontInfo;
	FontInfo = nullptr;
}

void Text::Draw()
{
	SDL_RenderCopy(Renderer, FontTexture, FontInfo, NULL);
}
