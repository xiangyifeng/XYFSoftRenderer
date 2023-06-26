#pragma once
#include "RenderWidget.h"
#include "QWidget"

namespace Ui {
class RenderWidget;
}

class RenderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
private:
    Ui::RenderWidget *ui;
};
