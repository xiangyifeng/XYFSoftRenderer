#include "Model.h"
#include <QDebug>

Model::Model(QString path) {
    LoadModel(path);
    center = {(minX + maxX) / 2.f, (minY + maxY) / 2.f, (minZ + maxZ) / 2.f};
}

void Model::Draw() {

    RendererDevice::GetInstance().textureList = textureList;

    for(int i = 0; i < meshList.size(); i++) {
        meshList[i].Draw();
    }
}

float Model::GetYRange() {
    return maxY - minY;
}

void Model::LoadModel(QString path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_GenNormals);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        isLoadSuccess = false;
        return;
    }
    QString modelName = path.mid(path.lastIndexOf('/') + 1);
    directory = path.mid(0, path.lastIndexOf('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshList.push_back(ProcessMesh(mesh, scene));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh res;
    vertexCount += mesh->mNumVertices;
    triangleCount += mesh->mNumFaces;
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        minX = std::min(minX, mesh->mVertices[i].x);
        minY = std::min(minY, mesh->mVertices[i].y);
        minZ = std::min(minZ, mesh->mVertices[i].z);
        maxX = std::max(maxX, mesh->mVertices[i].x);
        maxY = std::max(maxY, mesh->mVertices[i].y);
        maxZ = std::max(maxZ, mesh->mVertices[i].z);
        vertex.worldSpacePos = Coord3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = Vector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0]) {
            vertex.texCoord = Coord2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.texCoord = Coord2D(0.f, 0.f);
        }
        res.vertices.push_back(vertex);
    }
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            res.indices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        res.diffuseTextureIndex = LoadMaterialTextures(res, mat, aiTextureType_DIFFUSE);
        res.specularTextureIndex = LoadMaterialTextures(res, mat, aiTextureType_SPECULAR);
        res.ambientTextureIndex = LoadMaterialTextures(res, mat, aiTextureType_AMBIENT);
    }
    return res;
}

int Model::LoadMaterialTextures(Mesh& mesh,
                                aiMaterial* material,
                                aiTextureType type) {
    if(material->GetTextureCount(type) > 0) {
        aiString str;
        material->GetTexture(type, 0, &str);
        QString path = directory + '/' + str.C_Str();
        for(int i = 0; i < textureList.size(); i++) {
            if(textureList[i].path == path) {
                return i;
            }
        }
        Texture texture;
        if(texture.LoadFromImage(path)) {
            textureList.push_back(texture);
            return textureList.size() - 1;
        }
    }
    return -1;
}
