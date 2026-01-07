#pragma once

#include "core/Client.h"

#include "Envy/opengl/Texture2D.h"
#include "Envy/opengl/Pipeline.h"

class Material
{
public:
    Material();
    ~Material();

    GLResource<Envy::Texture2D> albedo;
    GLResource<Envy::Pipeline> pipeline;
};