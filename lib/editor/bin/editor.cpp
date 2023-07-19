#include "editor/editor.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include <pine/pine.hpp>

int main(int argc, char** argv) {
    pine::EngineSpecs specs;
    specs.working_directory = ".";
    specs.name = "Editor";
    specs.window_width = 1280;
    specs.window_height = 720;
    specs.start_maximized = true;
    specs.vsync = true;
    specs.resizable = true;
    specs.enable_gui = true;
    specs.fullscreen = true;

    // Create application
    pine::Editor editor;

    // Initialize engine and run application
    pine::Engine engine(specs);
    // engine.init();
    engine.run(editor);

    return 0;
}
