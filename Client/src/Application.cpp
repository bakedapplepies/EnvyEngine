#include "Application.h"

#include <fmt/format.h>

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
    m_GLFWUserData = std::make_unique<GLFWUserData>();
    glfwSetWindowUserPointer(m_window, m_GLFWUserData.get());
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(m_window, MouseCallback);
    glfwSwapInterval(0);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fmt::println("Failed to initialize GLAD");
        return false;
    }

    m_applicationIsActive = true;
    m_envyInstance = &envy_instance;
    
    return true;
}

void Application::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();

    m_window = nullptr;
    m_GLFWUserData.reset();
    m_applicationIsActive = false;

    m_renderer.reset();
    m_mainCamera = nullptr;
}

void Application::Run()
{
    m_renderer = std::make_unique<Renderer>(m_envyInstance);

    _LoadShaderPrograms();
    _LoadTexture2Ds();
    
    m_envyInstance->SetViewport(0, 0, 1280, 720);

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

    const Envy::ShaderProgram* vertexShader =
        m_envyInstance->GetShaderProgram(Path("src/shaders/default.vert").Str());
    const Envy::ShaderProgram* fragmentShader =
        m_envyInstance->GetShaderProgram(Path("src/shaders/default.frag").Str());

    Transform quadTransform;
        
    Material quadMaterial;
    quadMaterial.albedo = m_envyInstance->GetTexture2D(Path("resources/images/villager.png").Str());
    quadMaterial.pipeline = m_envyInstance->CreatePipeline();
    quadMaterial.pipeline->SetVertexProgram(vertexShader);
    quadMaterial.pipeline->SetFragmentProgram(fragmentShader);

    RenderCommand renderCommand {
        .vertexArray = quad,
        .vaoChunk = &quadVAOChunk,
        .material = &quadMaterial,
        .transform = &quadTransform
    };

    Camera camera;
    m_mainCamera = &camera;
    m_GLFWUserData->cameraInControl = m_mainCamera;
    camera.position = glm::vec3(0.0f, 0.0f, 2.0f);
    camera.viewDir = glm::vec3(0.0f, 0.0f, -1.0f);

    const Envy::Cubemap* skybox =
        m_envyInstance->CreateCubemap(Envy::TextureFormat::RGBA8,
                                      Path("resources/cubemaps/skybox/right.jpg").Str(),
                                      Path("resources/cubemaps/skybox/left.jpg").Str(),
                                      Path("resources/cubemaps/skybox/top.jpg").Str(),
                                      Path("resources/cubemaps/skybox/bottom.jpg").Str(),
                                      Path("resources/cubemaps/skybox/front.jpg").Str(),
                                      Path("resources/cubemaps/skybox/back.jpg").Str());

    float beginTime = glfwGetTime();
    float totalTime = 0.0f;
    float deltaTime = 0.0f;
    uint32_t totalFrames = 0;
    
    while (m_applicationIsActive)
    {
        deltaTime = glfwGetTime() - beginTime;
        totalTime += deltaTime;
        beginTime = glfwGetTime();

        m_envyInstance->ClearBuffer();
        m_envyInstance->ClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        m_renderer->Render(m_mainCamera, skybox, renderCommand);
        glfwSwapBuffers(m_window);

        _ProcessInput(deltaTime);

        if (totalTime >= 1.0f)
        {
            totalTime = 0.0f;
            fmt::println("FPS: {}", totalFrames);
            totalFrames = 0;
        }
        totalFrames++;
    }
}

void Application::_ProcessInput(float delta_time)
{
    glfwPollEvents();
    if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        m_applicationIsActive = false;
    }

    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_mainCamera->position += 4.0f * glm::normalize(glm::vec3(m_mainCamera->viewDir.x, 0.0f, m_mainCamera->viewDir.z)) * delta_time;
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_mainCamera->position -= 4.0f * glm::normalize(glm::vec3(m_mainCamera->viewDir.x, 0.0f, m_mainCamera->viewDir.z)) * delta_time;
    }
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_mainCamera->position += 4.0f * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_mainCamera->viewDir)) * delta_time;
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_mainCamera->position -= 4.0f * glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_mainCamera->viewDir)) * delta_time;
    }
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_mainCamera->position += 4.0f * glm::vec3(0.0f, 1.0f, 0.0f) * delta_time;
    }
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_mainCamera->position -= 4.0f * glm::vec3(0.0f, 1.0f, 0.0f) * delta_time;
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