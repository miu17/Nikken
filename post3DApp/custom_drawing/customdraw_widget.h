#ifndef CUSTOMDRAWWIDGET_H
#define CUSTOMDRAWWIDGET_H

#include <QtWidgets>
#include "interface_widget.h"

class QToolButton;
class QSpinBox;
class QSlider;

namespace post3dapp{
class CustomDrawGrView;

class CustomDrawWidget : public InterfaceWidget
{
    Q_OBJECT

public:

    CustomDrawWidget(QWidget *parent = nullptr);

//    virtual void sendActiveStatusToScene(){ }

public slots:

    void slotChangeViewScale();
    void zoomIn();
    void zoomOut();
    void slotChangeSliderValue(qreal factor);

protected:

    QToolButton *fitButton;
    QToolButton *zoomInIcon;
    QToolButton *zoomOutIcon;
    QSlider *zoomSlider;

    virtual CustomDrawGrView* getView() const = 0;
    virtual void createSceneView() = 0;
    virtual void createButtons();
    virtual void createLayout();

};
} // namespace post3dapp

#endif
