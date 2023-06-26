#include "Mesh.h"

void Mesh::Draw() {
    RendererDevice::GetInstance().vertexList = vertices;
    RendererDevice::GetInstance().indices = indices;
    RendererDevice::GetInstance().shader->material.diffuse = diffuseTextureIndex;
    RendererDevice::GetInstance().shader->material.specular = specularTextureIndex;
    RendererDevice::GetInstance().shader->material.ambient = ambientTextureIndex;
    RendererDevice::GetInstance().Render();
}