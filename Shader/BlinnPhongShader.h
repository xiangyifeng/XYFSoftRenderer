#pragma once
#include "../RendererCore/Shader.hpp"
#include "../RendererCore/MyTexture.h"


class BlinnPhongShader : public Shader {
public:
    virtual void VertexShader(Vertex& vertex) override;
    virtual void FragmentShader(Fragment& fragment) override;
};