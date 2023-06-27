#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int h, int w) : height(h), with(w), depthBuffer(w * h), colorBuffer(w, h, QImage::Format_RGB888) {
    colorBuffer.fill(QColor(0.f, 0.f, 0.f));
    std::fill(depthBuffer.begin(), depthBuffer.end(), 1.f);
}

// bool FrameBuffer::JudgeDepth(int x, int y, float depth) {
//     if(depth < depthBuffer[y * with + x]) {
//         depthBuffer[y * with + x] = depth;
//         return true;
//     }
//     return false;
// }

bool FrameBuffer::JudgeDepth(float x, float y, float depth) {
    if(depth < depthBuffer[y * with + x]) {
        depthBuffer[y * with + x] = depth;
        return true;
    }
    return false;
}

void FrameBuffer::SetPixel(int x, int y, Color color) {
    colorBuffer.setPixelColor(x, y, QColor(color.r, color.g, color.b));
}

bool FrameBuffer::SaveImage(QString filePath) {
    return colorBuffer.save(filePath);
}

void FrameBuffer::ClearBuffer(Color color) {
    std::fill(depthBuffer.begin(), depthBuffer.end(), 1.f);
    colorBuffer.fill(QColor(color.r * 255.f, color.g * 255.f, color.b * 255.f));
}
