#include <filesystem>
#include <memory>
#include <string>

#include "pine/pine.hpp"

#include "editor/editor_layer.hpp"

class Editor : public pine::Application
{
public:
    Editor(const pine::Application::Specification& specs,
        const std::string& project_path)
        : pine::Application(specs), m_project_path(project_path)
    {
        push_layer(new pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_project_path;
    std::filesystem::path m_storage_path;
};

std::unique_ptr<pine::Application> pine::create_application(int argc,
    char** argv)
{
    const auto project_path = [argc, argv]()
    {
        if (argc > 1)
        {
            return std::string(argv[1]);
        }
        else
        {
            return std::string(".");
        }
    }();

    pine::Application::Specification specs;
    specs.working_directory = ".";
    specs.name = "Editor";
    specs.window_width = 1280;
    specs.window_height = 720;
    specs.start_maximized = true;
    specs.vsync = true;
    specs.resizable = true;
    specs.enable_imgui = true;
    specs.fullscreen = true;

    return std::make_unique<Editor>(specs, project_path);
}

int main(int argc, char** argv)
{
    pine::Log::init();

    auto app = pine::create_application(argc, argv);
    app->run();

    return 0;
}
