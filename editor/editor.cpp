#include <filesystem>
#include <memory>
#include <string>

#include "pine/pine.hpp"

#include "editor/editor_layer.hpp"

class Editor : public pine::Application
{
public:
    Editor(const pine::ApplicationSpecs& specs, const std::string& project_path)
        : pine::Application(specs), m_project_path(project_path)
    {
        push_layer(new pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_project_path;
    std::filesystem::path m_storage_path;
};

class EditorFactory : public pine::ApplicationFactory
{
public:
    EditorFactory(int argc, char** argv) : m_argc(argc), m_argv(argv) {}
    
    virtual ~EditorFactory() = default;

    std::unique_ptr<pine::Application> create_application() override
    {
        const auto argc = m_argc;
        const auto argv = m_argv;

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

        pine::ApplicationSpecs specs;
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

private:
    int m_argc;
    char** m_argv;
};



int main(int argc, char** argv)
{
    pine::Log::init();

    EditorFactory factory(argc, argv);
    auto app = factory.create_application();
    app->run();

    return 0;
}
