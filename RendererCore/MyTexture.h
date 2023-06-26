#pragma once

#include <QImage>
#include <QString>
#include "DataDefine.hpp"

class Texture {
    enum {
        DIFFUSE,
        SPECULAR
    };
    int with;
    int height;
    QImage texture;
public:
    QString path;
    bool LoadFromImage(QString path);
    Color Sample2D(Coord2D coord);
};