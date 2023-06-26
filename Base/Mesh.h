#pragma once

#include "../RendererCore/DataDefine.hpp"
#include "../RendererCore/MyTexture.h"
#include "../RendererCore/RendererDevice.h"

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    int diffuseTextureIndex = -1;
    int specularTextureIndex = -1;
    int ambientTextureIndex = -1;
    void Draw();
};