#pragma once

namespace pine
{

class Timestep
{
public:
    Timestep(float time = 0.0f) : m_time(time) {}

    operator float() const { return m_time; }

    float get_seconds() const { return m_time; }
    float get_milliseconds() const { return m_time * 1000.0f; }

private:
    float m_time;
};

} // namespace pine
