#pragma once

#include "DataDefine.hpp"
#include <QImage>
#include <QString>
#include <QColor>

class FrameBuffer {
public:
    FrameBuffer(int h, int w);
    //bool JudgeDepth(int x, int y, float depth);
    bool JudgeDepth(float x, float y, float depth);
    void SetPixel(int x, int y, Color color);
    bool SaveImage(QString filePath);
    void ClearBuffer(Color color);
    QImage& GetImage() {return colorBuffer;}
private:
    int height;
    int with;
    std::vector<float> depthBuffer;
    QImage colorBuffer;
};