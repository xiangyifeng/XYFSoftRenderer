#pragma once

#include <vector>
#include "DataDefine.hpp"
#include "RendererDevice.h"

class RendererDevice;

class Shader {
public:
    Matrix4x4 model;
    Matrix4x4 view;
    Matrix4x4 projection;
    std::vector<Light> lightList;
    Material material;
    Coord3D eyePos;
    virtual void VertexShader(Vertex& vertex) = 0;
    virtual void FragmentShader(Fragment& fragment) = 0;
};