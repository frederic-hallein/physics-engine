#pragma once

#include <SDL2/SDL.h>

class RenderWindow
{
public:
    RenderWindow(const char* title, int width, int height);
    SDL_Renderer* getRenderer();
    void close();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
};