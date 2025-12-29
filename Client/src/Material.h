#pragma once

#include "Envy/opengl/Texture2D.h"
#include "Envy/opengl/Pipeline.h"

class Material
{
public:
    Material();
    ~Material();

    const Envy::Texture2D* albedo = nullptr;
    Envy::Pipeline* pipeline = nullptr;
};