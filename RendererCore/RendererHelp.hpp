#pragma once
#include "RendererDevice.h"
#include "DataDefine.hpp"
#include <bitset>

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

template<class T>
static inline float CalculateDistance(T point, T border)
{
    return glm::dot(point, border);
}

std::vector<Triangle> ConstructTriangle(std::vector<Vertex> vertexList)
{
    std::vector<Triangle> res;
    for(int i = 0; i < vertexList.size() - 2; i++)
    {
        int k = (i + 1) % vertexList.size();
        int m = (i + 2) % vertexList.size();
        Triangle tri{vertexList[0], vertexList[k], vertexList[m]};
        res.push_back(tri);
    }
    return res;
}

template<class T,size_t N>
static inline std::bitset<N> GetClipCode(T point, std::vector<T>& clip)
{
    std::bitset<N> res;
    for(int i = 0; i < N; i++)
        if(CalculateDistance(point, clip[i]) < 0) res.set(i, 1);
    return res;
}

static inline std::bitset<4> GetClipCode2(Coord4D point)
{
    std::bitset<4> res;
    if(point.x < -1) {
        res.set(0, 1);
    }
    if(point.x > 1) {
        res.set(1, 1);
    }
    if(point.y < -1) {
        res.set(2,1);
    }
    if(point.y > 1) {
        res.set(3,1);
    }
    return res;
}

static inline bool Inside(const Vector3D &line,const Vector3D &p) {
	return line.x * p.x + line.y * p.y + line.z < 0;
}

static inline Vertex ClipTriangleInterpolation(Vertex v1, Vertex v2, Vector3D line) {
    Vertex res;
    if (line.x == -1) {
		float weight = (-1 - v1.clipSpacePos.x) / (v2.clipSpacePos.x - v1.clipSpacePos.x);
		return CalculateInterpolation(v1, v2, weight);
	}
	if (line.x == 1) {
		float weight = (1 - v1.clipSpacePos.x) / (v2.clipSpacePos.x - v1.clipSpacePos.x);
		return CalculateInterpolation(v1, v2, weight);
	}
	if (line.y == -1) {
		float weight = (-1 - v1.clipSpacePos.y) / (v2.clipSpacePos.y - v1.clipSpacePos.y);
		return  CalculateInterpolation(v1, v2, weight);
	}
	if (line.y == 1) {
		float weight = (1 - v1.clipSpacePos.y) / (v2.clipSpacePos.y - v1.clipSpacePos.y);
		return CalculateInterpolation(v1, v2, weight);
	}
    return res;
}