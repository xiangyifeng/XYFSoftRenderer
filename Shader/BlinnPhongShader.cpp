#include "BlinnPhongShader.h"

void BlinnPhongShader::VertexShader(Vertex& vertex) {
    vertex.worldSpacePos = Coord3D(model * Coord4D(vertex.worldSpacePos, 1.f));
    vertex.clipSpacePos = projection * view * Coord4D(vertex.worldSpacePos, 1.f);
    vertex.normal = Matrix3x3(glm::transpose(glm::inverse( model))) * vertex.normal;
}

void BlinnPhongShader::FragmentShader(Fragment& fragment) {
    Color ambientColor = {0.2f, 0.2f, 0.2f};
    Color diffuseColor = {0.6f, 0.6f, 0.6f};
    Color specularColor = {1.f, 1.f, 1.f};
    if(material.diffuse != -1) {
        diffuseColor = RendererDevice::GetInstance().textureList[material.diffuse].Sample2D(fragment.texCoord);
    }
    if(material.specular != -1) {
        specularColor = RendererDevice::GetInstance().textureList[material.specular].Sample2D(fragment.texCoord);
    }
    if(material.ambient != -1) {
        ambientColor = RendererDevice::GetInstance().textureList[material.ambient].Sample2D(fragment.texCoord);
    }
    Light light = lightList[0];
    Vector3D normal = glm::normalize(fragment.normal);
    Vector3D viewDir = glm::normalize(eyePos - fragment.worldSpacePos);
    Color ambient = ambientColor * light.lightColor;
    Color diffuse = std::max(0.f, glm::dot(normal, light.dir)) * diffuseColor * light.lightColor;
    Color specular = std::pow(std::max(0.f, glm::dot(normal, glm::normalize(viewDir + light.dir))), material.shininess) * specularColor * lightList[0].lightColor;
    Color result = ambient  + diffuse + specular;
    if(result.x  > 255.f) result.x = 255.f;
    if(result.y  > 255.f) result.y = 255.f;
    if(result.z  > 255.f) result.z = 255.f;
    fragment.fragmentColor = result;
}
