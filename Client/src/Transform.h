#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

    glm::mat4 GetModelMatrix() const;
};