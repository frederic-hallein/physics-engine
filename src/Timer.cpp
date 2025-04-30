#include "Timer.hpp"

Timer::Timer()
    : m_deltaTime(0.0f),
      m_frameDuration(0)
{
    m_frameStart = std::chrono::high_resolution_clock::now();
    m_lastFrame = m_frameStart;
}

void Timer::startFrame()
{
    m_frameStart = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(m_frameStart - m_lastFrame).count();
    m_lastFrame = m_frameStart;
}

float Timer::getDeltaTime() const
{
    return m_deltaTime;
}

void Timer::capFrameRate(int targetFPS)
{
    int targetFrameTime = 1000 / targetFPS;

    // Calculate frame duration
    auto frameEnd = std::chrono::high_resolution_clock::now();
    m_frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - m_frameStart).count();

    // Sleep to cap the frame rate
    if (m_frameDuration < targetFrameTime)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(targetFrameTime - m_frameDuration));
        m_frameDuration = targetFrameTime; // Adjust frame duration to match the target
    }
}

void Timer::printFrameDuration() const
{
    std::cout << "Frame Time: " << m_frameDuration << " ms" << '\n';
}