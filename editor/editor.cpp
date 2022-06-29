#include <filesystem>
#include <memory>
#include <string>

#include "pine/pine.hpp"

#include "editor/editor_layer.hpp"

class Editor : public pine::Application
{
public:
    Editor(const pine::Application::Specification& specs,
        const std::string& projectPath)
        : pine::Application(specs), m_ProjectPath(projectPath)
    {
        PushLayer(new pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_ProjectPath;
    std::filesystem::path m_StoragePath;
};

std::unique_ptr<pine::Application> pine::CreateApplication(int argc,
    char** argv)
{
    const auto projectPath = [argc, argv]()
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
    specs.WorkingDirectory = ".";
    specs.Name = "Editor";
    specs.WindowWidth = 1280;
    specs.WindowHeight = 720;
    specs.StartMaximized = true;
    specs.VSync = true;
    specs.Resizable = true;
    specs.EnableImGui = true;
    specs.Fullscreen = true;

    return std::make_unique<Editor>(specs, projectPath);
}

int main(int argc, char** argv)
{
    pine::Log::Init();

    auto app = pine::CreateApplication(argc, argv);
    app->Run();

    return 0;
}
