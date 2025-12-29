#include "Application.h"
#include "Envy/EnvyInstance.h"

int main()
{
    // Object creation
    WindowConfig windowConfig {
        .name = "EnvyClient",
        .width = 1920,
        .height = 1080
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