#include "Application.h"
#include "Envy/EnvyInstance.h"

int main(int argc, char** argv)
{
    // Object creation
    WindowConfig windowConfig {
        .name = "EnvyClient",
        .width = 1280,
        .height = 720
    };
    Application window;
    Envy::EnvyInstance envyInstance;
    
    window.Create(windowConfig, envyInstance);
    envyInstance.Init();

    // Execution
    window.Run();

    // Clean-up
    envyInstance.Shutdown();
    window.Destroy();
    
    return 0;
}