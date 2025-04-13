#include <iostream>

#include "PhysicsEngine.hpp"

PhysicsEngine::PhysicsEngine(const char* name, int width, int height)
    // : m_renderWindow(name, width, height)
{
    // m_renderer = m_renderWindow.getRenderer();
    std::cout << "PhysicsEngine created.\n";
};

void PhysicsEngine::handleEvents()
{
    // SDL_PollEvent(&m_event);
    // switch (m_event.type){
    //     case SDL_QUIT:
    //         m_isRunning = false;
    //         break;

    //     default:
    //         break;
    // }
}

bool PhysicsEngine::running()
{
    return m_isRunning;
}

void PhysicsEngine::render()
{
    // SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    // SDL_RenderClear(m_renderer);
    // SDL_RenderPresent(m_renderer);
}

void PhysicsEngine::close()
{
    // m_renderWindow.close();
    // m_renderer = NULL;
    std::cout << "PhysicsEngine closed.\n";
}