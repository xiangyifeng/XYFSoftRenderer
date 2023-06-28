#pragma once

#include "DataDefine.hpp"
#include <QImage>
#include <QString>
#include <QColor>


class FrameBuffer {
public:
    FrameBuffer(int h, int w);
    bool JudgeDepth(int x, int y, float depth);
    bool JudgeDepthWithMSAA(int x, int y, int i, float depth);
    void SetPixel(int x, int y, Color color);
    void AddPixel(int x, int y, Color color);
    void SetPixelWithMSAA(int x, int y, int i, Color color);
    bool SaveImage(QString filePath);
    void ClearBuffer(Color color);
    void RenderImage();
    void SetMSAA(bool IsMSAA);
    QImage& GetImage() {return colorBuffer;}
    bool MSAA = false;
private:
    int height;
    int with;
    std::vector<float> depthBuffer;
    std::vector<float> MSAADepthBuffer;
    std::vector<Color> MSAAColorBuffer;
    QImage colorBuffer;
};