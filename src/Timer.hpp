#pragma once
#include <chrono>
#include <thread>
#include <iostream>

class Timer
{
public:
    Timer();

    void startFrame();
    float getDeltaTime() const;
    void capFrameRate(int targetFPS);
    void printFrameDuration() const;

private:
    std::chrono::high_resolution_clock::time_point m_frameStart;
    std::chrono::high_resolution_clock::time_point m_lastFrame;
    float m_deltaTime;
    int m_frameDuration;
};