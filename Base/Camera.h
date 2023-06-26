#pragma once

#include "../RendererCore/DataDefine.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
    Camera(float _aspect, float _zFar) : aspect(_aspect), zFar(_zFar){}
    Vector3D position;
    Vector3D target;
    float aspect;
    float fov = 60.f;
    float zNear = 1.f;
    float zFar;
    void SetModel(Coord3D modelCenter, float yRange);
    Matrix4x4 GetViewMatrix();
    Matrix4x4 GetProjectionMatrix();
    void CloseToTarget(float distance);
    void MoveTarget(Vector2D motion);
    void RotateAroundTarget(Vector2D motion);
};