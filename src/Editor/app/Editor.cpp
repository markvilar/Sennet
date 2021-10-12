#include "Sennet/Sennet.hpp"

#include "Editor/EditorLayer.hpp"

namespace Sennet
{

class Editor : public Application
{
public:
    Editor() : Application("Sennet-Editor") { PushLayer(new EditorLayer()); }

    ~Editor() {}
};

Application* CreateApplication() { return new Editor(); }

} // namespace Sennet

int main(int argc, char** argv)
{
    Sennet::Log::Init();

    auto app = Sennet::CreateApplication();
    app->Run();

    return 0;
}
