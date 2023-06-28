#include "FrameBuffer.h"
#include <omp.h>

FrameBuffer::FrameBuffer(int h, int w) : height(h), with(w), depthBuffer(w * h), colorBuffer(w, h, QImage::Format_RGB888), MSAADepthBuffer((4 * w + 100) * h), MSAAColorBuffer(4 * w * h) {
    colorBuffer.fill(QColor(0.f, 0.f, 0.f));
    std::fill(depthBuffer.begin(), depthBuffer.end(), 1.f);
    std::fill(MSAADepthBuffer.begin(), MSAADepthBuffer.end(), 1.f);
    std::fill(MSAAColorBuffer.begin(), MSAAColorBuffer.end(), Color(0.f, 0.f, 0.f));
}

bool FrameBuffer::JudgeDepth(int x, int y, float depth) {
    if(depth < depthBuffer[y * with + x]) {
        depthBuffer[y * with + x] = depth;
        return true;
    }
    return false;
}

bool FrameBuffer::JudgeDepthWithMSAA(int x, int y, int i, float depth) {
    if(depth < MSAADepthBuffer[(y * with + x) * 4 + i]) {
        MSAADepthBuffer[(y * with + x) * 4 + i] = depth;
        return true;
    }
    return false;
}

void FrameBuffer::SetPixel(int x, int y, Color color) {
    colorBuffer.setPixelColor(x, y, QColor(color.r, color.g, color.b));
}

void FrameBuffer::AddPixel(int x, int y, Color color) {
    QColor currentColor = colorBuffer.pixelColor(x, y);
    colorBuffer.setPixelColor(x, y, QColor(color.r + currentColor.red(), color.g + currentColor.green(), color.b + currentColor.blue()));
}

void FrameBuffer::SetPixelWithMSAA(int x, int y, int i, Color color) {
    MSAAColorBuffer[(y * with + x) * 4 + i] = color;
}

bool FrameBuffer::SaveImage(QString filePath) {
    return colorBuffer.save(filePath);
}

void FrameBuffer::ClearBuffer(Color color) {
    std::fill(depthBuffer.begin(), depthBuffer.end(), 1.f);
    colorBuffer.fill(QColor(color.r * 255.f, color.g * 255.f, color.b * 255.f));
    if(MSAA) {
        std::fill(MSAADepthBuffer.begin(), MSAADepthBuffer.end(), 1.f);
        std::fill(MSAAColorBuffer.begin(), MSAAColorBuffer.end(), Color(0.f, 0.f, 0.f));
    }
}

void FrameBuffer::RenderImage() {
    int len = MSAAColorBuffer.size();
    #pragma omp parallel for
    for(int i = 0; i < len; i = i + 4) {
        Color color = (MSAAColorBuffer[i] + MSAAColorBuffer[i + 1] + MSAAColorBuffer[i + 2] + MSAAColorBuffer[i + 3]) / 4.f;
        int t = i / 4;
        colorBuffer.setPixelColor(t % with, t / with, QColor(color.r, color.g, color.b));
    }
}

void FrameBuffer::SetMSAA(bool IsMSAA) {
    MSAA = IsMSAA;
}
