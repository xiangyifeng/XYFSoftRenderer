#pragma once

#include "Mesh.h"
#include "../RendererCore/DataDefine.hpp"
#include "../RendererCore/MyTexture.h"
#include "assimp/postprocess.h"
#include <assimp/scene.h>
#include "assimp/Importer.hpp"

class Model {
public:
    Model(QString path);
    void Draw();
    Coord3D center;
    int triangleCount = 0;
    int vertexCount = 0;
    float GetYRange();
    bool isLoadSuccess = true;
private:
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float minZ = FLT_MAX;
    float maxX = FLT_MIN;
    float maxY = FLT_MIN;
    float maxZ = FLT_MIN;
    std::vector<Mesh> meshList;
    std::vector<Texture> textureList = {};
    QString directory;
    void LoadModel(QString path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    int LoadMaterialTextures(Mesh& mesh, aiMaterial* material, aiTextureType type);
};