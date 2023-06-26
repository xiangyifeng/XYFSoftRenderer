#include "XYFRenderWidget.h"
#include "RenderWidget.h"
#include <QPainter>
#include "RendererDevice.h"

RenderWidget::RenderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenderWidget)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    // ui->FPSLabel->setStyleSheet("background:transparent");
    // ui->FPSLabel->setVisible(false);
}

void RenderWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, RendererDevice::GetInstance().GetBuffer());
}

RenderWidget::~RenderWidget() {
    delete ui;
}
