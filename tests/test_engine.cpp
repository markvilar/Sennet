#include <pine/pine.hpp>

struct TestApplication
{
    void init() {}
    void update(const pine::Timestep& timestep) {}
    void shutdown() {}

    void on_gui_render() {}
    void on_event(const pine::Event& event) {}
};

int main(const int argc, const char** argv)
{
    TestApplication app;

    pine::Engine engine;
    engine.run(app);
}
