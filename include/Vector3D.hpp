#pragma once

class Vector3D
{
public:
    float x, y, z;

    Vector3D() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3D(float xCoord, float yCoord, float zCoord) :
        x(xCoord),
        y(yCoord),
        z(zCoord) {}
    Vector3D(const Vector3D& v);

};