#pragma once

#include<QImage>
#include<QColor>
#include<QString>
#include <string>
#include <any>
#include <cassert>
#include <cmath>
#include <iostream>
#include <array>
#include "glm/glm.hpp"

using Color = glm::vec3;
using Vector2D = glm::vec2;
using Vector3D = glm::vec3;
using VectorI3D = glm::ivec3;
using Vector4D = glm::vec4;
using VectorI4D = glm::ivec4;
using Coord2D = glm::vec2;
using CoordI2D = glm::ivec2;
using Coord3D = glm::vec3;
using CoordI3D = glm::ivec3;
using Coord4D = glm::vec4;
using CoordI4D = glm::ivec4;
using Matrix4x4 = glm::mat4;
using Matrix3x3 = glm::mat3;

enum RenderMode {
    FACE,
    EDGE
};

struct Vertex {
    Coord3D worldSpacePos;
    Coord4D clipSpacePos;
    Coord4D ndcSpacePos;
    Coord2D screenSpacePos;
    float screenDepth;
    Vector3D normal;
    Coord2D texCoord;
};

using Triangle = std::array<Vertex, 3>;
using Line = std::array<Coord2D, 2>;

struct Fragment {
    Coord3D worldSpacePos;
    Coord2D screenSpacePos;
    float screenDepth;
    Color fragmentColor;
    Vector3D normal;
    Coord2D texCoord;
};

struct Light {
    Coord4D pos;
    Vector3D dir;
    Color lightColor;
};

struct Material {
    int specular;
    int diffuse;
    int ambient;
    float shininess;
};
