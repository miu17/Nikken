#ifndef INTERFACEWIDGET_H
#define INTERFACEWIDGET_H

#include <QWidget>

namespace post3dapp{

class InterfaceWidget : public QWidget
{
    Q_OBJECT

public:

    InterfaceWidget(QWidget *parent = 0) : QWidget(parent) { }
    virtual ~InterfaceWidget(){}

    virtual void sendActiveStatusToScene() { }

signals:

    void drawingFloorChanged(const QString &);
    void drawingFrameChanged(const QString &);

};
} // namespace post3dapp

#endif
