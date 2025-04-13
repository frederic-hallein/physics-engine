#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "Vector3D.hpp"
#include "PhysicsEngine.hpp"

const int unsigned SCREEN_WIDTH = 1080;
const int unsigned SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
    PhysicsEngine physicsEngine("Physics Engine", SCREEN_WIDTH, SCREEN_HEIGHT);
    while (physicsEngine.isRunning())
    {
        physicsEngine.handleEvents();
        physicsEngine.render();
    }
    physicsEngine.close();
    return 0;
}
