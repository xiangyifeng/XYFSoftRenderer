#include "Camera.h"

void Camera::SetModel(Coord3D modelCenter, float yRange) {
    target = modelCenter;
    position = modelCenter;
    position.z = yRange / std::tan(glm::radians(fov) / 2);
}

Matrix4x4 Camera::GetViewMatrix() {
    Matrix4x4 res = glm::lookAt(position, target, {0.f, 1.f, 0.f});
    return res;
}

Matrix4x4 Camera::GetProjectionMatrix() {
    Matrix4x4 res = glm::perspective(fov, aspect, zNear, zFar);
    return res;
}

void Camera::CloseToTarget(float distance) {
    Vector3D formTarget = position-target;
    float radius = glm::length(formTarget);
    float yaw = (float)std::atan2(formTarget.x, formTarget.z);
    float pitch = (float)std::asin(formTarget.y / radius);
    Vector3D offset;
    radius *= (float)std::pow(0.95, distance);
    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));
    position = target + offset;
}

void Camera::MoveTarget(Vector2D motion)
{
    Vector3D fromPosition = target-position;
    Vector3D forward = glm::normalize(fromPosition);
    Vector3D left = glm::normalize(glm::cross({0.0f,1.0f,0.0f},forward));
    Vector3D up = glm::normalize(glm::cross(forward,left));

    float distance = glm::length(fromPosition);
    float factor = distance * (float)tan(glm::radians(fov) / 2) * 2;
    Vector3D deltaX = factor * aspect * motion.x * left;
    Vector3D deltaY = factor * motion.y * up;
    target += (deltaX+deltaY);
    position += (deltaX+deltaY);
}

void Camera::RotateAroundTarget(Vector2D motion)
{
    Vector3D formTarget = position-target;
    float radius = glm::length(formTarget);
    float yaw = (float)std::atan2(formTarget.x, formTarget.z);
    float pitch = (float)std::asin(formTarget.y / radius);
    float factor = (float)M_PI * 2.f;
    Vector3D offset;
    yaw -= motion.x * factor;
    pitch += motion.y * factor;

    if(pitch + M_PI_2 < FLT_EPSILON) pitch = - glm::radians(89.9f);
    if(pitch - M_PI_2 > FLT_EPSILON) pitch = glm::radians(89.9f);

    offset.x = (radius * (float)std::cos(pitch) * (float)std::sin(yaw));
    offset.y = (radius * (float)std::sin(pitch));
    offset.z = (radius * (float)std::cos(pitch) * (float)std::cos(yaw));

    position = target + offset;
}