#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad.h>

class Mesh
{
public:
    Mesh() = default;
    Mesh(const char* meshPath);

    void draw();
    void deleteMesh();

private:
    unsigned int VAO, VBO;
    unsigned int vertexCount;
};