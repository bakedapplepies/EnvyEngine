#include "Application.h"

#include <fmt/format.h>

#include "GLFWCallbacks.h"
#include "Path.h"

Application::Application()
{}

Application::~Application()
{}

bool Application::Create(const WindowConfig& window_config, const Envy::EnvyInstance& envy_instance)
{
    // TODO: Add Wayland checks
    glfwInitHint(GLFW_WAYLAND_LIBDECOR, GLFW_WAYLAND_DISABLE_LIBDECOR);

    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwSetErrorCallback(ErrorCallback);

    // TODO: Add WindowConfig option to specify vulkan or opengl
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_window = glfwCreateWindow(window_config.width,
                                window_config.height,
                                window_config.name,
                                NULL,
                                NULL);
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::println("Failed to initialize GLAD");
        return false;
    }

    m_applicationIsActive = true;
    m_envyInstance = &envy_instance;
    m_renderer = std::make_unique<Renderer>(&envy_instance);

    return true;
}

void Application::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
    m_applicationIsActive = false;

    m_renderer.reset();
}

void Application::Run()
{
    _LoadShaderPrograms();
    _LoadTexture2Ds();
    
    m_envyInstance->SetViewport(0, 0, 1920, 1080);

    std::array<Envy::Vertex, 4> vertices = {
        Envy::Vertex {
            .position = glm::vec3(-1.0f,  1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 0.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 1.0f,  1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 1.0f,  1.0f)
        },
        Envy::Vertex {
            .position = glm::vec3( 1.0f, -1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 1.0f,  0.0f)
        },
        Envy::Vertex {
            .position = glm::vec3(-1.0f, -1.0f,  0.0f),
            .normal   = glm::vec3( 0.0f,  0.0f,  0.0f),
            .uv       = glm::vec2( 0.0f,  0.0f)
        }
    };

    std::array<GLuint, 6> indices = {
        0, 1, 2,
        0, 2, 3
    };

    const Envy::VertexArray* quad =
        m_envyInstance->CreateVertexArray(vertices.data(),
                                          vertices.size(),
                                          indices.data(),
                                          indices.size());

    Envy::VAOChunk quadVAOChunk = {
        .elementsOffset = 0,
        .elementsCount = indices.size(),
        .vertex_offset = 0
    };

    Material quadMaterial;
    quadMaterial.albedo = m_envyInstance->GetTexture2D(Path("resources/images/villager.png").Str());
    quadMaterial.pipeline = m_envyInstance->CreatePipeline();
    quadMaterial.pipeline->SetVertexProgram(
        m_envyInstance->GetShaderProgram(Path("src/shaders/default.vert").Str()));
    quadMaterial.pipeline->SetFragmentProgram(
        m_envyInstance->GetShaderProgram(Path("src/shaders/default.frag").Str()));

    RenderCommand renderCommand {
        .vertexArray = quad,
        .vaoChunk = &quadVAOChunk,
        .material = &quadMaterial
    };

    float beginTime = glfwGetTime();
    float totalTime = 0.0f;
    uint32_t totalFrames = 0;
    
    while (m_applicationIsActive)
    {
        totalTime += glfwGetTime() - beginTime;
        beginTime = glfwGetTime();

        m_envyInstance->ClearBuffer();
        m_envyInstance->ClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        m_renderer->Render(renderCommand);
        glfwSwapBuffers(m_window);

        _ProcessInput();

        if (totalTime >= 1.0f)
        {
            totalTime = 0.0f;
            fmt::println("{}", totalFrames);
            totalFrames = 0;
        }
        totalFrames++;
    }
}

void Application::_ProcessInput()
{
    glfwPollEvents();
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_applicationIsActive = false;
    }
}

void Application::_LoadShaderPrograms() const
{
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::VERTEX,
        Path("src/shaders/default.vert").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::FRAGMENT,
        Path("src/shaders/default.frag").Str());
    m_envyInstance->LoadShaderProgram(
        Envy::ShaderType::COMPUTE,
        Path("src/shaders/default.comp").Str());
}

void Application::_LoadTexture2Ds() const
{
    m_envyInstance->LoadTexture2D(
        Envy::TextureFormat::RGBA8,
        Path("resources/images/villager.png").Str());
}