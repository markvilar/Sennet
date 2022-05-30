#include <filesystem>
#include <memory>
#include <string>

#include "Pine/Pine.hpp"

#include "Editor/EditorLayer.hpp"

class Editor : public Pine::Application
{
public:
    Editor(const Pine::Application::Specification& specs,
        const std::string& projectPath)
        : Pine::Application(specs), m_ProjectPath(projectPath)
    {
        PushLayer(new Pine::EditorLayer());
    }

    ~Editor() {}

private:
    std::filesystem::path m_ProjectPath;
    std::filesystem::path m_StoragePath;
};

std::unique_ptr<Pine::Application> Pine::CreateApplication(int argc,
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

    return std::make_unique<Editor>(specs, projectPath);
}

int main(int argc, char** argv)
{
    Pine::Log::Init();

    auto app = Pine::CreateApplication(argc, argv);
    app->Run();

    return 0;
}
