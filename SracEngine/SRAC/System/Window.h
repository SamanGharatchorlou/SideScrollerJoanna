#pragma once

class Window
{
public:
	Window() : mWindow(nullptr) { };
	~Window();

	void init(const char* title, Vector2D<int> size);

	SDL_Renderer* createRenderer();

	SDL_Window* get() const { return mWindow; }
	Vector2D<int> size() const;

	void setTitle(const char* title);


private:
	SDL_Window* mWindow;
};