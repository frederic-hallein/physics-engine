#include <iostream>
#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

#include "PhysicsEngine.hpp"

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
    PhysicsEngine physicsEngine("Physics Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    while(physicsEngine.running())
    {
        physicsEngine.handleEvents();
        physicsEngine.render();
    }

    physicsEngine.close();
    return 0;
}
