#include "editor/editor_layer.hpp"
#include "pine/pine.hpp"

#include <filesystem>
#include <memory>
#include <string>

class Editor : public pine::Application
{
public:
    Editor(const pine::ApplicationSpecs& specs,
        const std::string& project_path_)
        : pine::Application(specs),
          project_path(project_path_)
    {
        push_layer(new pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path project_path;
    std::filesystem::path storage_path;
};

class EditorFactory : public pine::ApplicationFactory
{
public:
    EditorFactory(int argc_, char** argv_) : argc(argc_), argv(argv_) {}

    virtual ~EditorFactory() = default;

    std::unique_ptr<pine::Application> create_application() override
    {
        const auto count = argc;
        const auto arguments = argv;

        const auto project_path = [count, arguments]()
        {
            if (count > 1)
            {
                return std::string(arguments[1]);
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
        specs.enable_gui = true;
        specs.fullscreen = true;

        return std::make_unique<Editor>(specs, project_path);
    }

private:
    int argc;
    char** argv;
};

int main(int argc, char** argv)
{
    pine::Log::init();

    EditorFactory factory(argc, argv);
    auto app = factory.create_application();
    app->run();

    return 0;
}
