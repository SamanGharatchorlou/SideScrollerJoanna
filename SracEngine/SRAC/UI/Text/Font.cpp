#include "pch.h"
#include "Font.h"

#include "Graphics/Renderer.h"

Font::~Font()
{
	if (mTexture)
		SDL_DestroyTexture(mTexture);

	if(mFont)
		TTF_CloseFont(mFont);
}


bool Font::loadFromFile(const BasicString& filePath, int ptSize)
{
	mRenderer = Renderer::Get();

	//Open the font
	mFont = TTF_OpenFont(filePath.c_str(), ptSize);
	if (mFont != nullptr)
	{
		mFontName = FileManager::Get()->getItemName(fs::path(filePath.c_str()));
		mPtSize = ptSize;
		return true;
	}
	else
	{
		DebugPrint(Warning, "Failed to load font at '%s'! SDL_ttf Error: %s", filePath.c_str(), TTF_GetError());
		return false;
	}
}


void Font::resize(int ptSize)
{
	// Font must be closed and reloaded
	TTF_CloseFont(mFont);

	mFont = TTF_OpenFont(FileManager::Get()->findFile(FileManager::Font, mFontName.c_str()).c_str(), ptSize);
	mPtSize = ptSize;
}


void Font::setText(const BasicString& text)
{
	if (mFont != nullptr && !text.empty())
	{
		//Render text surface
		SDL_Surface* textSurface = TTF_RenderText_Blended(mFont, text.c_str(), colour);
		if (!textSurface)
		{
			DebugPrint(Warning, "Unable to render text surface for text: %s! SDL_ttf Error: %s", text.c_str(), TTF_GetError());
		}
		else
		{
			renderTextSurface(textSurface);
		}
	}
	else
	{
		if(mFont == nullptr)
			DebugPrint(Error, "Font has not beed loaded for text: %s, Call Font::loadFromFile first", text);
	}
}





void Font::setWrappedText(const BasicString& text, int width)
{
	if (mFont != nullptr && !text.empty())
	{
		SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(mFont, text.c_str(), colour, width);

		if (!textSurface)
		{
			DebugPrint(Warning, "Unable to render wrapped text surface for text: %s! SDL_ttf Error: %s", text.c_str(), TTF_GetError());
		}
		else
		{
			renderTextSurface(textSurface);
		}
	}
	else
	{
		if (mFont == nullptr)
			DebugPrint(Error, "Font has not beed loaded for text: %s, Call Font::loadFromFile first", text);
	}
}



void Font::render(const VectorF position) const
{
	SDL_Rect renderQuad = { static_cast<int>(position.x),
							static_cast<int>(position.y),
							mSize.x, mSize.y };

	SDL_RenderCopyEx(mRenderer->sdlRenderer(), mTexture, nullptr, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}


// -- Private Functions -- //
void Font::renderTextSurface(SDL_Surface* textSurface)
{
	if (mTexture)
		SDL_DestroyTexture(mTexture);

	//Create texture from surface pixels
	mRenderer->lock();
	mTexture = SDL_CreateTextureFromSurface(mRenderer->sdlRenderer(), textSurface);
	mRenderer->unlock();

	if (mTexture == nullptr)
		DebugPrint(Warning, "Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());
	else
		mSize = Vector2D<int>(textSurface->w, textSurface->h);

	// loaded surface no longer needed
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
}