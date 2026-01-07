#include "Envy/opengl/ResourceManager.h"

#if _WIN32
#   include <windows.h>
#endif

#include <sstream>
#include <fstream>
#include <cassert>

#include <stb/stb_image.h>

#include "Envy/core/Assert.h"
#include "Envy/core/Logging.h"

ENVY_NAMESPACE_START

ResourceManager::ResourceManager()
{}

ResourceManager::~ResourceManager()
{
    m_shaderPrograms.clear();
    m_texture2DIndices.clear();
    m_texture2Ds.Clear();
    m_VAOs.Clear();
    m_pipelines.Clear();
    m_UBOs.Clear();
    m_indirectBuffers.Clear();
    m_cubemaps.Clear();
}

void ResourceManager::LoadShaderProgram(ShaderType shader_type, std::string_view file_path)
{
    if (m_shaderPrograms.find(file_path.data()) != m_shaderPrograms.end())
    {
        ENVY_WARN("Shader program \"{}\" is already loaded.", file_path.data());
        return;
    }
    std::string shaderSource = ReadFile(file_path);
    m_shaderPrograms[file_path.data()] = ShaderProgram(shader_type, shaderSource);
}

void ResourceManager::LoadTexture2D(TextureFormat texture_format, std::string_view file_path)
{
    // Note: Cannot delete a texture yet as deleted textures will leave
    // empty spaces in the vector (which can be fixed)

    if (m_texture2DIndices.find(file_path.data()) != m_texture2DIndices.end())
    {
        ENVY_WARN("Texture2D \"{}\" is already loaded.", file_path.data());
        return;
    }
    
    int width, height, channels;  // TODO: channels is not used
    stbi_uc* imageData = ReadImage(file_path, &width, &height, &channels);

    m_texture2DIndices[file_path.data()] = m_texture2Ds.Size();
    m_texture2Ds.AddBack(Texture2D(width, height, texture_format));
    m_texture2Ds.GetBack()->UploadImage(imageData, width, height);

    stbi_image_free(imageData);
}

const ShaderProgram* ResourceManager::GetShaderProgram(std::string_view file_path) const
{
    bool foundProgram = m_shaderPrograms.find(file_path.data()) != m_shaderPrograms.end();
    ENVY_ASSERT(foundProgram, "Shader program path not recognized.");

    return (foundProgram) ? &m_shaderPrograms.at(file_path.data()) : nullptr;
}

const Resource<Texture2D> ResourceManager::GetTexture2D(std::string_view file_path) const
{
    bool foundTexture = m_texture2DIndices.find(file_path.data()) != m_texture2DIndices.end();
    ENVY_ASSERT(foundTexture, "Texture path not recognized.");

    return (foundTexture) ? m_texture2Ds.GetAt(m_texture2DIndices.at(file_path.data()))
                          : Resource<Texture2D>::empty;
}

const Resource<Texture2D> ResourceManager::CreateTexture2DEmpty(TextureFormat texture_format,
                                                                int width,
                                                                int height)
{
    m_texture2Ds.AddBack(Texture2D(width, height, texture_format));

    return m_texture2Ds.GetBack();
}

const Resource<VertexArray> ResourceManager::CreateVAO(const Vertex* vertices, uint32_t n_vertices,
                                                       const GLuint* indices, uint32_t n_indices)
{
    m_VAOs.AddBack(VertexArray(vertices, n_vertices, indices, n_indices));

    return m_VAOs.GetBack();
}

Resource<Pipeline> ResourceManager::CreatePipeline()
{
    m_pipelines.AddBack(Pipeline());

    return m_pipelines.GetBack();
}

Resource<Pipeline> ResourceManager::CreatePipeline(const ShaderProgram* vert_program,
                                                   const ShaderProgram* frag_program)
{
    m_pipelines.AddBack(Pipeline(vert_program, frag_program));

    return m_pipelines.GetBack();
}

const Resource<Framebuffer> ResourceManager::CreateFramebuffer(int width, int height)
{
    m_framebuffers.AddBack(Framebuffer(width, height));

    return m_framebuffers.GetBack();
}

const Resource<UniformBuffer> ResourceManager::CreateUBO(uint32_t ubo_block_size, uint32_t binding)
{
    m_UBOs.AddBack(UniformBuffer(ubo_block_size, binding));

    return m_UBOs.GetBack();
}

const Resource<IndirectBuffer> ResourceManager::CreateIndirectBuffer(uint32_t command_count,
                                                                     const IndirectCommand* commands)
{
    m_indirectBuffers.AddBack(IndirectBuffer(command_count, commands));

    return m_indirectBuffers.GetBack();
}

const Resource<Cubemap>
ResourceManager::CreateCubemap(TextureFormat format,
                               const std::array<std::string_view, 6>& texture2D_paths)
{
    std::array<uint8_t*, 6> data;
    int width, height, channels;
    for (uint32_t i = 0; i < 6; i++)
    {
        data[i] = ReadImage(texture2D_paths[i], &width, &height, &channels, false);
    }

    m_cubemaps.AddBack(Cubemap(width, height, format, data));

    for (uint32_t i = 0; i < 6; i++)
    {
        stbi_image_free(data[i]);
    }

    return m_cubemaps.GetBack();
}

std::string ResourceManager::ReadFile(std::string_view file_path)
{
    std::ifstream infile(file_path.data());
    ENVY_ASSERT(infile.is_open(), "File not open.");

    std::string line;
    std::stringstream ss;
    while (std::getline(infile, line))
    {
        ss << line << '\n';
    }

    return ss.str();
}

uint8_t* ResourceManager::ReadImage(std::string_view file_path,
                                    int* width, int* height, int* channels,
                                    bool flip)
{
#ifndef _WIN32
    FILE* f = fopen(file_path.data(), "rb");
#else
    std::string s(file_path);
    std::wstring filePathW;
    filePathW.resize(s.length());
    int newSize = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), s.size(), const_cast<wchar_t *>(filePathW.c_str()), filePathW.length());
    filePathW.resize(newSize);
    FILE* f = _wfopen(filePathW.c_str(), L"rb");
#endif

    stbi_set_flip_vertically_on_load(flip);
    stbi_uc* pixels = stbi_load_from_file(f, width, height, channels, STBI_rgb_alpha);
    ENVY_ASSERT(pixels != nullptr,
                fmt::format("Failed to load texture image data.\n{}", stbi_failure_reason()));
    fclose(f);

    return pixels;
}

ENVY_NAMESPACE_END