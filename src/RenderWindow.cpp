#include <iostream>
// #include <SDL2/SDL.h>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* title, int width, int height)
{
    // if (SDL_Init(SDL_INIT_VIDEO) < 0)
    //     std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << '\n';

    // m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
    //                             SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    // if (m_window == NULL)
    //     std::cout << "SDL could not create window! SDL Error: " << SDL_GetError() << '\n';

    // m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    // if (m_renderer == NULL)
    //     std::cout << "SDL could not create renderer! SDL Error: " << SDL_GetError() << '\n';

    std::cout << "RenderWindow created.\n";
}

// SDL_Renderer* RenderWindow::getRenderer()
// {
//     return m_renderer;
// }

void RenderWindow::close()
{
    // SDL_DestroyRenderer(m_renderer);
    // SDL_DestroyWindow(m_window);
    // m_renderer = NULL;
    // m_window = NULL;
    // SDL_Quit();
    std::cout << "RenderWindow closed.\n";
}
