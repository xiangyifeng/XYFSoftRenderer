#pragma once

#include <vector>
#include "DataDefine.hpp"
#include "FrameBuffer.h"
#include "MyTexture.h"
#include "Shader.hpp"


class Shader;

struct EdgeEquation {
    Vector3D I, J, K;
    bool topLeftFlag[3];
    int twoArea;
    float delta;
    EdgeEquation(const Triangle& tri);
    Vector3D GetResult(float x, float y);
    void UpX(Vector3D& res);
    void UpY(Vector3D& res);
    Vector3D GetBarycentric(Vector3D val);
};

class RendererDevice {
public:
    RenderMode renderMode = FACE;
    bool faceCulling = false;
    bool multiThread = false;
    bool MSAA = false;
    std::vector<Vertex> vertexList;
    std::vector<unsigned> indices;
    std::vector<Texture> textureList;
    std::unique_ptr<Shader> shader;
    Color clearColor = {0.f, 0.f, 0.f};
    Color PointColor;
    Color LineColor;
    RendererDevice(int w, int h);
    void ClearBuffer();
    QImage& GetBuffer();
    bool SaveImage(QString filePath);
    void Render();
    static void Init(int w, int h);
    static RendererDevice& GetInstance(int w = 800, int h = 600);
    void SetMultiThread(bool isSet);
    void SetFaceCulling(bool isCulling);
private:
    static RendererDevice* instance;
    int with;
    int height;
    FrameBuffer framebuffer;
    FrameBuffer MSAAFramebuffer;
    std::array<Vector4D, 6> viewPlanes;
    std::array<Vector3D, 4> screenLines;
    std::array<float, 4> xOffset = {0.25f, 0.25f, 0.75f, 0.75f};
    std::array<float, 4> yOffset = {0.25f, 0.75f, 0.25f, 0.75f};
    void ProcessTriangle(Triangle& tri);
    void RasterizationTriangle(Triangle& tri);
    void WireFrameTriangle(Triangle& tri);
    void PointTriangle(Triangle& tri);
    void DrawLine(Line& line);
    void ConvertToScreen(Triangle& tri);
    void ExecutePerspectiveDivision(Triangle& tri);
    Fragment ConstructFragment(int x, int y, float z, Triangle& tri, Vector3D& barycentric);
    CoordI4D GetBoundingBox(Triangle& tri);
    std::vector<Triangle> ClipTriangle(Triangle& tri);
    std::optional<Line> ClipLine(Line& line);
};