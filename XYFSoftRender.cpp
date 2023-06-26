#include "XYFSoftRender.h"
#include <QFileDialog>
#include <QMessageBox>
#include "RendererDevice.h"
#include "BlinnPhongShader.h"
#include <string>
#include <QWheelEvent>
#include "XYFRenderWidget.h"

XYFSoftRender::XYFSoftRender(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_XYFSoftRender)
    , camera((float)DEFAULT_WIDTH / DEFAULT_HEIGHT, FIXED_CAMERA_FAR)
{
    ui->setupUi(this);
    myColorDialog = new QColorDialog();
    InitDevice();
    InitLight();
    SetLightDir();
    connect(&timer, &QTimer::timeout, this, &XYFSoftRender::Render);
    connect(ui->RedLineEdit, SIGNAL(returnPressed()), this, SLOT(on_lightColorChange()));
    connect(ui->BlueLineEdit, SIGNAL(returnPressed()), this, SLOT(on_lightColorChange()));
    connect(ui->GreenLineEdit, SIGNAL(returnPressed()), this, SLOT(on_lightColorChange()));
    RenderWidget *widget=new RenderWidget();
    ui->RenderWidgetLayout->addWidget(widget);
    timer.start(1);
}

XYFSoftRender::~XYFSoftRender()
{
    delete ui; 
}

void XYFSoftRender::InitDevice() {
    RendererDevice::Init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    RendererDevice::GetInstance().shader = std::make_unique<BlinnPhongShader>();
    RendererDevice::GetInstance().shader->lightList.push_back(Light());
}

void XYFSoftRender::LoadModel(QString path) {
    Model* newModel = new Model(path);
    if(!newModel->isLoadSuccess) {
        QMessageBox::critical(this, "Error", "Model loading error!");
        delete newModel;
        return;
    }
    if(model != nullptr) {
        delete model;
    }
    model = newModel;
    ResetCamera();
}

void XYFSoftRender::ResetCamera() {
    camera.SetModel(model->center, model->GetYRange());
    modelMatrix = Matrix4x4(1.f);
}

void XYFSoftRender::SetCameraParam() {

}

void XYFSoftRender::Render() {
    if(model == nullptr) return;
    RendererDevice::GetInstance().ClearBuffer();
    int nowTime = QTime::currentTime().msecsSinceStartOfDay();
    if(lastFrameTime != 0) {
        deltaTime = nowTime - lastFrameTime;
        ui->FPSLabel->setText(QStringLiteral("FPS : ")+QString::number(1000.0 / deltaTime, 'f', 0));
    }
    lastFrameTime = nowTime;
    ProcessInput();
    SetCameraParam();
    RendererDevice::GetInstance().shader->model = modelMatrix;
    RendererDevice::GetInstance().shader->view = camera.GetViewMatrix();
    RendererDevice::GetInstance().shader->projection = camera.GetProjectionMatrix();
    RendererDevice::GetInstance().shader->eyePos = camera.position;
    RendererDevice::GetInstance().shader->material.shininess = 150.f;
    model->Draw();

    update();
}

void XYFSoftRender::InitLight() {
    ui->RedLineEdit->setText("255");
    ui->GreenLineEdit->setText("255");
    ui->BlueLineEdit->setText("255");
    QPalette pal = ui->editColorButton->palette();
    pal.setColor(QPalette::Button,QColor(ui->RedLineEdit->text().toInt(), ui->GreenLineEdit->text().toInt(), ui->BlueLineEdit->text().toInt()));
    ui->editColorButton->setPalette(pal);
    ui->editColorButton->setAutoFillBackground(true);
    ui->editColorButton->setFlat(true);
    lightDir = glm::normalize(Vector3D(1.f, 1.f, 1.f));
    SetLightColor(QColor(255, 255, 255));
}

void XYFSoftRender::SetLightColor(QColor color) {
    lightColor.setRed(color.red());
    lightColor.setGreen(color.green());
    lightColor.setBlue(color.blue());
    RendererDevice::GetInstance().shader->lightList[0].lightColor = Color(color.red(), color.green(), color.blue());
}

void XYFSoftRender::SetLightDir() {
    ui->PitchLabel->setText(QString::number(ui->PitchSlider->value()));
    ui->YawLabel->setText(QString::number(ui->YawSlider->value()));
    ui->RollLabel->setText(QString::number(ui->RollSlider->value()));
    RendererDevice::GetInstance().shader->lightList[0].dir = lightDir;
}



void XYFSoftRender::on_editColorButton_clicked() {
    QColor color = myColorDialog->getColor(lightColor, this, "Select Light Color");
    if(color.isValid()) {
        ui->RedLineEdit->setText(QString::number(color.red()));
        ui->GreenLineEdit->setText(QString::number(color.green()));
        ui->BlueLineEdit->setText(QString::number(color.blue()));
        QPalette pal = ui->editColorButton->palette();
        pal.setColor(QPalette::Button, color);
        ui->editColorButton->setPalette(pal);
        ui->editColorButton->setAutoFillBackground(true);
        ui->editColorButton->setFlat(true);
        SetLightColor(color);
    }
       
}

void XYFSoftRender::on_lightColorChange() {
    QColor color = QColor(ui->RedLineEdit->text().toInt(), ui->GreenLineEdit->text().toInt(), ui->BlueLineEdit->text().toInt());
    QPalette pal = ui->editColorButton->palette();
    pal.setColor(QPalette::Button, color);
    ui->editColorButton->setPalette(pal);
    ui->editColorButton->setAutoFillBackground(true);
    ui->editColorButton->setFlat(true);
    SetLightColor(color);
}

void XYFSoftRender::on_PitchSlider_valueChanged(int value) {
    ui->PitchLabel->setText(QString::number(value));
    pitchValue = value;
    float pitchRadian = pitchValue * glm::pi<float>() / 180.f;
    float yawRadian = yawValue * glm::pi<float>() / 180.f;
    float rollRadian = rollValue * glm::pi<float>() / 180.f;
    Vector3D dir = Matrix3x3(1.f, 0.f, 0.f, 0.f, glm::cos(pitchRadian), -glm::sin(pitchRadian), 0.f, glm::sin(pitchRadian), glm::cos(pitchRadian)) * lightDir;
    dir = Matrix3x3(glm::cos(yawRadian), 0.f, glm::sin(yawRadian), 0.f, 1.f, 0.f, -glm::sin(yawRadian), 0.f, glm::cos(yawRadian)) * dir;
    dir = Matrix3x3(glm::cos(rollRadian), -glm::sin(rollRadian), 0.f, glm::sin(rollRadian), glm::cos(rollRadian), 0.f, 0.f, 0.f, 1.f) * dir;
    RendererDevice::GetInstance().shader->lightList[0].dir = dir;
}

void XYFSoftRender::on_YawSlider_valueChanged(int value) {
    ui->YawLabel->setText(QString::number(value));
    yawValue = value;
    float pitchRadian = pitchValue * glm::pi<float>() / 180.f;
    float yawRadian = yawValue * glm::pi<float>() / 180.f;
    float rollRadian = rollValue * glm::pi<float>() / 180.f;
    Vector3D dir = Matrix3x3(1.f, 0.f, 0.f, 0.f, glm::cos(pitchRadian), -glm::sin(pitchRadian), 0.f, glm::sin(pitchRadian), glm::cos(pitchRadian)) * lightDir;
    dir = Matrix3x3(glm::cos(yawRadian), 0.f, glm::sin(yawRadian), 0.f, 1.f, 0.f, -glm::sin(yawRadian), 0.f, glm::cos(yawRadian)) * dir;
    dir = Matrix3x3(glm::cos(rollRadian), -glm::sin(rollRadian), 0.f, glm::sin(rollRadian), glm::cos(rollRadian), 0.f, 0.f, 0.f, 1.f) * dir;
    RendererDevice::GetInstance().shader->lightList[0].dir = dir;
}

void XYFSoftRender::on_RollSlider_valueChanged(int value) {
    ui->RollLabel->setText(QString::number(value));
    rollValue = value;
    float pitchRadian = pitchValue * glm::pi<float>() / 180.f;
    float yawRadian = yawValue * glm::pi<float>() / 180.f;
    float rollRadian = rollValue * glm::pi<float>() / 180.f;
    Vector3D dir = Matrix3x3(1.f, 0.f, 0.f, 0.f, glm::cos(pitchRadian), -glm::sin(pitchRadian), 0.f, glm::sin(pitchRadian), glm::cos(pitchRadian)) * lightDir;
    dir = Matrix3x3(glm::cos(yawRadian), 0.f, glm::sin(yawRadian), 0.f, 1.f, 0.f, -glm::sin(yawRadian), 0.f, glm::cos(yawRadian)) * dir;
    dir = Matrix3x3(glm::cos(rollRadian), -glm::sin(rollRadian), 0.f, glm::sin(rollRadian), glm::cos(rollRadian), 0.f, 0.f, 0.f, 1.f) * dir;
    RendererDevice::GetInstance().shader->lightList[0].dir = dir;
}

void XYFSoftRender::on_MultiThreadCheckBox_clicked() {
    if(ui->MultiThreadCheckBox->isChecked()) {
        RendererDevice::GetInstance().SetMultiThread(true);
    }
    else {
        RendererDevice::GetInstance().SetMultiThread(false);
    }
}

void XYFSoftRender::on_FaceCullingCheckBox_clicked() {
    if(ui->FaceCullingCheckBox->isChecked()) {
        RendererDevice::GetInstance().SetFaceCulling(true);
    }
    else {
        RendererDevice::GetInstance().SetFaceCulling(false);
    }
}

void XYFSoftRender::on_actionOpen_triggered() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Model File", "", "OBJ(*.obj)");
    if(!filePath.isEmpty()) {
        LoadModel(filePath);
    }
}

// void XYFSoftRender::paintEvent(QPaintEvent *) {
//     QPainter painter(this);
//     painter.drawImage(0, 0, RendererDevice::GetInstance().GetBuffer());
// }

void XYFSoftRender::wheelEvent(QWheelEvent *event) {
    QPoint numDegrees = event->angleDelta();
    camera.CloseToTarget(numDegrees.y() / 120);
}

void XYFSoftRender::mousePressEvent(QMouseEvent *event) {
    currentBtns = event->buttons();
    currentPos = event->pos();
    lastPos = {0,0};
}

void XYFSoftRender::mouseReleaseEvent(QMouseEvent *event) {
    currentBtns = event->buttons();
}

void XYFSoftRender::mouseMoveEvent(QMouseEvent* event) {
    currentPos = event->pos();
}

void XYFSoftRender::ProcessInput() {
    if((currentBtns & Qt::LeftButton) || (currentBtns & Qt::RightButton))
    {
        if(!lastPos.isNull())
        {
            Vector2D motion = {(float)(currentPos - lastPos).x(),(float)(currentPos - lastPos).y()};
            motion.x = motion.x / DEFAULT_WIDTH;
            motion.y = motion.y / DEFAULT_HEIGHT;
            if(currentBtns & Qt::LeftButton)
            {
                camera.RotateAroundTarget(motion);
            }
            if(currentBtns & Qt::RightButton)
            {
                camera.MoveTarget(motion);
            }
        }
        lastPos = currentPos;
    }
}