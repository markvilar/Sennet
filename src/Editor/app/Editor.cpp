#include "Sennet/Sennet.hpp"

#include "Editor/EditorLayer.hpp"

#include <filesystem>
#include <string_view>

class Editor : public Sennet::Application
{
public:
    Editor(const Sennet::Application::Specification& specs,
        const std::string_view projectPath)
        : Sennet::Application(specs), m_ProjectPath(projectPath)
    {
        PushLayer(new Sennet::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_ProjectPath;
    std::filesystem::path m_StoragePath;
    // TODO: Sennet::UserPreferences m_Preferences;
};

Sennet::Application* Sennet::CreateApplication(int argc, char** argv)
{
    std::string_view projectPath;
    if (argc > 1)
    {
        projectPath = argv[1];
    }

    Sennet::Application::Specification specs;
    specs.WorkingDirectory = ".";
    specs.Name = "Editor";
    specs.WindowWidth = 1280;
    specs.WindowHeight = 720;
    specs.StartMaximized = true;
    specs.VSync = true;
    specs.Resizable = true;
    specs.EnableImGui = true;
    specs.Fullscreen = false;

    return new Editor(specs, projectPath);
}

int main(int argc, char** argv)
{
    Sennet::Log::Init();

    auto app = Sennet::CreateApplication(argc, argv);
    app->Run();

    return 0;
}
