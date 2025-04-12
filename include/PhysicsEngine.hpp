#pragma once

#include <SDL2/SDL.h>

#include "RenderWindow.hpp"

class PhysicsEngine
{
public:
    PhysicsEngine(const char* name, int width, int height);

    bool running();
    void handleEvents();
    void render();
    void close();

private:
    bool m_isRunning = true;
    SDL_Event m_event;
    RenderWindow m_renderWindow;
    SDL_Renderer* m_renderer;

};