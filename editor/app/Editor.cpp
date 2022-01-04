#include <filesystem>
#include <string_view>

#include "Pine/Pine.hpp"

#include "Editor/EditorLayer.hpp"

class Editor : public Pine::Application
{
public:
    Editor(const Pine::Application::Specification& specs,
        const std::string_view projectPath)
        : Pine::Application(specs), m_ProjectPath(projectPath)
    {
        PushLayer(new Pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_ProjectPath;
    std::filesystem::path m_StoragePath;
    // TODO: Pine::UserPreferences m_Preferences;
};

Pine::Application* Pine::CreateApplication(int argc, char** argv)
{
    std::string_view projectPath;
    if (argc > 1)
    {
        projectPath = argv[1];
    }

    Pine::Application::Specification specs;
    specs.WorkingDirectory = ".";
    specs.Name = "Editor";
    specs.WindowWidth = 1280;
    specs.WindowHeight = 720;
    specs.StartMaximized = true;
    specs.VSync = true;
    specs.Resizable = true;
    specs.EnableImGui = true;
    specs.Fullscreen = true;

    return new Editor(specs, projectPath);
}

int main(int argc, char** argv)
{
    Pine::Log::Init();

    auto app = Pine::CreateApplication(argc, argv);
    app->Run();

    return 0;
}
