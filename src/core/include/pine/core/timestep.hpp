#pragma once

namespace pine
{
class Timestep
{
public:
    Timestep(float time_ = 0.0f) : time(time_) {}

    operator float() const { return time; }

    float get_seconds() const { return time; }
    float get_milliseconds() const { return time * 1000.0f; }

private:
    float time;
};

} // namespace pine
