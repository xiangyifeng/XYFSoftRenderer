#pragma once
#include "RendererDevice.h"
#include "DataDefine.hpp"

//当像素在边上时用Top-Left Fill Rule
static inline bool JudgeOnTopLeftEdge(CoordI2D v0, CoordI2D v1) {
    return (v0.y > v1.y || (v0.y == v1.y && (v0.x > v1.x)));
}

static inline bool JudgeInsideTriangle(EdgeEquation& triEdge, Vector3D res) {
    bool flag = true;
    if(res.x == 0) {
        flag &= triEdge.topLeftFlag[0];
    }
    if(res.y == 0) {
        flag &= triEdge.topLeftFlag[1];
    }
    if(res.z == 0) {
        flag &= triEdge.topLeftFlag[2];
    }
    return flag && ((res.x >= 0 && res.y >= 0 && res.z >= 0) || (res.x <= 0 && res.y <= 0 && res.z <= 0));
}

static inline bool JudgeInsideTriangle2(Triangle& tri, float x, float y) {
    Vector2D pos = {x, y};
    auto AB = tri[1].screenSpacePos - tri[0].screenSpacePos;
    auto BC = tri[2].screenSpacePos - tri[1].screenSpacePos;
    auto CA = tri[0].screenSpacePos - tri[2].screenSpacePos;
    auto AP = pos - tri[0].screenSpacePos;
    auto BP = pos - tri[1].screenSpacePos;
    auto CP = pos - tri[2].screenSpacePos;
    return ((AB.x * AP.y - AB.y * AP.x) > 0 && (BC.x * BP.y - BC.y * BP.x > 0) && (CA.x * CP.y - CA.y * CP.x > 0));
}

template<class T>
static inline T CalculateInterpolation(T a, T b, T c, Vector3D& barycentric) {
    return a * barycentric.x + b * barycentric.y + c * barycentric.z;
}

template<class T>
static inline T CalculateInterpolation(T a, T b, float alpha) {
    return a * (1 - alpha) + b * alpha;
}

static inline CoordI2D CalculateInterpolation(CoordI2D a, CoordI2D b, float alpha) {
    CoordI2D res;
    res.x = static_cast<int>(a.x * (1 - alpha) + b.x * alpha + 0.5f);
    res.y = static_cast<int>(a.y * (1 - alpha) + b.y * alpha + 0.5f);
    return res;
}

static inline Vertex CalculateInterpolation(Vertex a, Vertex b, float alpha) {
    Vertex res;
    res.worldSpacePos = CalculateInterpolation<Coord3D>(a.worldSpacePos, b.worldSpacePos, alpha);
    res.clipSpacePos = CalculateInterpolation<Coord4D>(a.clipSpacePos, b.clipSpacePos, alpha);
    res.normal = CalculateInterpolation<Vector3D>(a.normal, b.normal, alpha);
    res.texCoord = CalculateInterpolation<Coord2D>(a.texCoord, b.texCoord, alpha);
    return res;
}

template<class T>
static inline T CorrectPerspectiveInterpolation(std::vector<T> attribute, Triangle& tri, Vector3D& barycentric)
{
    return (barycentric.x * attribute[0] / tri[0].clipSpacePos.w + barycentric.y * attribute[1] / tri[1].clipSpacePos.w + barycentric.z * attribute[2] / tri[2].clipSpacePos.w) / (barycentric.x / tri[0].clipSpacePos.w + barycentric.y / tri[1].clipSpacePos.w + barycentric.z / tri[2].clipSpacePos.w);
}
