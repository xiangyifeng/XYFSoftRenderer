#include "RendererDevice.h"
#include "RendererHelp.hpp"
#include <vector>
#include <omp.h>


RendererDevice* RendererDevice::instance = nullptr;

EdgeEquation::EdgeEquation(const Triangle& tri) {
    I = {
        tri[1].screenSpacePos.x - tri[0].screenSpacePos.x,
        tri[2].screenSpacePos.x - tri[1].screenSpacePos.x,
        tri[0].screenSpacePos.x - tri[2].screenSpacePos.x    
    };
    J = {
        tri[0].screenSpacePos.y - tri[1].screenSpacePos.y,
        tri[1].screenSpacePos.y - tri[2].screenSpacePos.y,
        tri[2].screenSpacePos.y - tri[0].screenSpacePos.y    
    };
    K = {
        tri[0].screenSpacePos.x * tri[1].screenSpacePos.y - tri[0].screenSpacePos.y * tri[1].screenSpacePos.x,
        tri[1].screenSpacePos.x * tri[2].screenSpacePos.y - tri[1].screenSpacePos.y * tri[2].screenSpacePos.x,
        tri[2].screenSpacePos.x * tri[0].screenSpacePos.y - tri[2].screenSpacePos.y * tri[0].screenSpacePos.x
    };
    topLeftFlag[0] = JudgeOnTopLeftEdge(tri[0].screenSpacePos, tri[1].screenSpacePos);
    topLeftFlag[1] = JudgeOnTopLeftEdge(tri[1].screenSpacePos, tri[2].screenSpacePos);
    topLeftFlag[2] = JudgeOnTopLeftEdge(tri[2].screenSpacePos, tri[0].screenSpacePos);

    twoArea = K[0] + K[1] + K[2];
    delta = 1.f / twoArea;
}

Vector3D EdgeEquation::GetResult(float x, float y) {
    Vector3D res = I * y + J * x + K;
    return res;
}

void EdgeEquation::UpX(Vector3D& res) {
    res += J;
}

void EdgeEquation::UpY(Vector3D& res) {
    res += I;
}

Vector3D EdgeEquation::GetBarycentric(Vector3D val) {
    Vector3D res;
    res.x = val.y * delta;
    res.y = val.z * delta;
    res.z = val.x * delta;
    return res;
}

RendererDevice::RendererDevice(int w, int h) : shader(nullptr), with(w), height(h), framebuffer(w, h), MSAAFramebuffer(2 * w, 2 * h) {
    //near
    viewPlanes[0] = {0.f, 0.f, 1.f, 1.f};
    //far
    viewPlanes[1] = {0.f, 0.f, -1.f, 1.f};
    //left
    viewPlanes[2] = {1.f, 0.f, 0.f, 1.f};
    //right
    viewPlanes[3] = {-1.f, 0.f, 0.f, 1.f};
    //top
    viewPlanes[4] = {0.f, 1.f, 0.f, 1.f};
    //bottom
    viewPlanes[5] = {0.f, -1.f, 0.f, 1.f};
}

void RendererDevice::ClearBuffer() {
    framebuffer.ClearBuffer(clearColor);
}

QImage& RendererDevice::GetBuffer() {
    return framebuffer.GetImage();
}

bool RendererDevice::SaveImage(QString filePath) {
    return framebuffer.SaveImage(filePath);
}

void RendererDevice::Render() {
    std::vector<Triangle> triangleList;
    for(int i = 0; i < indices.size(); i += 3) {
        assert(i + 1 < indices.size() && i + 2 < indices.size());
        triangleList.push_back({vertexList[indices[i]], vertexList[indices[i + 1]], vertexList[indices[i + 2]]});
    }
    if(multiThread) {
        #pragma omp parallel for
        for(int i = 0; i < triangleList.size(); i++) {
            ProcessTriangle(triangleList[i]);
        }
    }
    else {
        for(int i = 0; i < triangleList.size(); i++) {
            ProcessTriangle(triangleList[i]);
        }
    }
}

void RendererDevice::Init(int w, int h) {
    GetInstance(w, h);
}

RendererDevice& RendererDevice::GetInstance(int w, int h) {
    if (instance == nullptr) {
        instance = new RendererDevice(w, h);
    }
    return *instance;
}

void RendererDevice::SetMultiThread(bool isSet) {
    multiThread = isSet;
}

void RendererDevice::SetFaceCulling(bool isCulling) {
    faceCulling = isCulling;
}

void RendererDevice::ProcessTriangle(Triangle& tri) {
    for(int i = 0; i < 3; i++) {
        shader->VertexShader(tri[i]);
    }
    //TO DO:Clip Triangle
    ExecutePerspectiveDivision(tri);
    ConvertToScreen(tri);
    if(renderMode == FACE) RasterizationTriangle(tri);
    //TO DO: WireframeTriangle and PointTriangle
}

void RendererDevice::RasterizationTriangle(Triangle& tri) {
    CoordI4D boundingBox = GetBoundingBox(tri);
    int xMin = boundingBox.x;
    int yMin = boundingBox.y;
    int xMax = boundingBox.z;
    int yMax = boundingBox.w;
    EdgeEquation edge(tri);
    if(faceCulling && edge.twoArea <= 0) {
        return;
    }
    if(edge.twoArea == 0) return;
    Fragment frag;
    Vector3D cy = edge.GetResult(xMin + 0.5f, yMin + 0.5f);
    for(int y = yMin; y <= yMax; y++) {
        Vector3D cx = cy;
        for(int x = xMin; x <= xMax; x++) {
            if(MSAA) {
                int count = 0;
                for(int i = 0; i < 4; i++) {
                    float sampleX = x + xOffset[i];
                    float sampleY = y + yOffset[i];
                    Vector3D sampleRes = edge.GetResult(sampleX, sampleY);
                    if(JudgeInsideTriangle(edge, sampleRes)) {
                        Vector3D barycentric = edge.GetBarycentric(sampleRes);
                        float depth = CalculateInterpolation<float>(tri[0].screenDepth, tri[1].screenDepth, tri[2].screenDepth, barycentric);
                        if(framebuffer.JudgeDepth(2 * x, 2 * y, depth)) {
                            count++;
                        }
                    }
                }
                if(count > 0) {
                    Vector3D barycentric = edge.GetBarycentric(cx);
                    float depth = CalculateInterpolation<float>(tri[0].screenDepth, tri[1].screenDepth, tri[2].screenDepth, barycentric);
                    frag = ConstructFragment(x, y, depth, tri, barycentric);
                    shader->FragmentShader(frag);
                    framebuffer.SetPixel(x, y, frag.fragmentColor * (count / 4.f));
                }
            }
            else {
                if(JudgeInsideTriangle(edge, cx)) {
                    Vector3D barycentric = edge.GetBarycentric(cx);
                    float depth = CalculateInterpolation<float>(tri[0].screenDepth, tri[1].screenDepth, tri[2].screenDepth, barycentric);
                    if(framebuffer.JudgeDepth(x, y, depth)) {
                        frag = ConstructFragment(x, y, depth, tri, barycentric);
                        shader->FragmentShader(frag);
                        framebuffer.SetPixel(x, y, frag.fragmentColor);
                    }
                }
            }
            edge.UpX(cx);
        }
        edge.UpY(cy);
    }
}

void RendererDevice::WireFrameTriangle(Triangle& tri) {}

void RendererDevice::PointTriangle(Triangle& tri) {}

void RendererDevice::DrawLine(Line& line) {}

void RendererDevice::ConvertToScreen(Triangle& tri) {
    for(int i = 0; i < 3; i++) {
        // float x = static_cast<int>(0.5f * with + tri[i].ndcSpacePos.x * with);
        // float y = static_cast<int>(0.5f * height + tri[i].ndcSpacePos.y * height);
        tri[i].screenSpacePos.x = static_cast<int>(0.5f * with + tri[i].ndcSpacePos.x * with) - 800;
        tri[i].screenSpacePos.y = static_cast<int>(0.5f * height + tri[i].ndcSpacePos.y * height) - 250;
        tri[i].screenDepth = tri[i].ndcSpacePos.z;
    }
}

void RendererDevice::ExecutePerspectiveDivision(Triangle& tri) {
    for(int i = 0; i < 3; i++)  {
        tri[i].ndcSpacePos.x = tri[i].clipSpacePos.x / tri[i].clipSpacePos.w;
        tri[i].ndcSpacePos.y = tri[i].clipSpacePos.y / tri[i].clipSpacePos.w;
        tri[i].ndcSpacePos.z = tri[i].clipSpacePos.z / tri[i].clipSpacePos.w;
    }
}

Fragment RendererDevice::ConstructFragment(int x,
                                           int y,
                                           float z,
                                           Triangle& tri,
                                           Vector3D& barycentric) {
    Fragment res;
    res.screenSpacePos.x = x;
    res.screenSpacePos.y = y;
    res.screenDepth = z;      
    res.normal =  CorrectPerspectiveInterpolation<Vector3D>({tri[0].normal, tri[1].normal, tri[2].normal}, tri, barycentric);  
    res.texCoord = CorrectPerspectiveInterpolation<Coord2D>({tri[0].texCoord, tri[1].texCoord, tri[2].texCoord}, tri, barycentric);
    res.worldSpacePos = CalculateInterpolation<Vector3D>(tri[0].worldSpacePos, tri[1].worldSpacePos, tri[2].worldSpacePos, barycentric);                 
    return res;
}

CoordI4D RendererDevice::GetBoundingBox(Triangle& tri) {
    int xMin = with - 1;
    int yMin = height - 1;
    int xMax = 0;
    int yMax = 0;
    for(int i = 0; i < 3; i++) {
        xMin = std::min(xMin, static_cast<int>(tri[i].screenSpacePos.x));
        yMin = std::min(yMin, static_cast<int>(tri[i].screenSpacePos.y));
        xMax = std::max(xMax, static_cast<int>(tri[i].screenSpacePos.x) + 1);
        yMax = std::max(yMax, static_cast<int>(tri[i].screenSpacePos.y) + 1);
    }
    CoordI4D res(xMin > 0 ? xMin : 0, yMin > 0 ? yMin : 0, xMax < with - 1 ? xMax : with - 1, yMax < height - 1 ? yMax : height - 1);
    return res;
}

std::vector<Triangle> RendererDevice::ClipTriangle(Triangle& tri) {
    return std::vector<Triangle>();
}

std::optional<Line> RendererDevice::ClipLine(Line& line) {
    return std::optional<Line>();
}
