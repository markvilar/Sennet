#include <pine/pine.hpp>

int main(int argc, char** argv)
{
    pine::Log::init();

    pine::WindowSpecs specs;
    const auto window = pine::create_window(specs);
    window->init();
    window->poll_events();
    window->swap_buffers();

    return 0;
};
