#pragma once
#include "ui_XYFSoftRender.h"
#include <QMainWindow>
#include "Base/Camera.h"
#include "Base/Model.h"
#include "DataDefine.hpp"
#include <QTimer>
#include <QPainter>
#include <QColorDialog>
#include "RenderWidget.h"


#define DEFAULT_WIDTH 2560.f
#define DEFAULT_HEIGHT 1440.f
#define FIXED_CAMERA_FAR 100.f

class XYFSoftRender : public QMainWindow {
    Q_OBJECT
    
public:
    XYFSoftRender(QWidget* parent = nullptr);
    ~XYFSoftRender();
    void InitDevice();
    void LoadModel(QString path);
    void ResetCamera();
    void SetCameraParam();
    void Render();
    void InitLight();
    void SetLightColor(QColor color);
    void SetLightDir();
    void ProcessInput();
    Camera camera;
private:
    Ui_XYFSoftRender* ui;
    Model* model = nullptr;
    Matrix4x4 modelMatrix;
    int lastFrameTime = 0;
    int deltaTime = 0;
    QTimer timer;
    QColor lightColor;
    Vector3D lightDir;
    QColorDialog* myColorDialog;
    int pitchValue = 0;
    int yawValue = 0;
    int rollValue = 0;
    QPoint lastPos;
    QPoint currentPos;
    Qt::MouseButtons currentBtns;

protected:
    //void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void on_actionOpen_triggered();
    void on_editColorButton_clicked();
    void on_lightColorChange();
    void on_PitchSlider_valueChanged(int value);
    void on_YawSlider_valueChanged(int value);
    void on_RollSlider_valueChanged(int value);
    void on_MultiThreadCheckBox_clicked();
    void on_FaceCullingCheckBox_clicked();
    void on_MSAACheckBox_clicked();
    void on_WireFrameCheckBox_clicked();
};