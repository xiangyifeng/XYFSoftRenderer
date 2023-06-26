#include "MyTexture.h"

bool Texture::LoadFromImage(QString path) {
    this->path = path;
    if(texture.load(path)) {
        texture.mirror();
        with = texture.width();
        height = texture.height();
        return true;
    }
    return false;
}

Color Texture::Sample2D(Coord2D coord) {
    int x = static_cast<int>(coord.x * with - 0.5) % with;
    int y = static_cast<int>(coord.y * height - 0.5) % height;
    return Color(texture.pixelColor(x, y).red() / 255.f, texture.pixelColor(x, y).green() / 255.f, texture.pixelColor(x, y).blue() / 255.f);
}
