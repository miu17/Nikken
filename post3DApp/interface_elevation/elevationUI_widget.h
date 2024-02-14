#ifndef ELEVATIONUIDRAWWIDGET_H
#define ELEVATIONUIDRAWWIDGET_H

#include "customdraw_widget.h"

class QToolButton;

namespace post3dapp{
class ReferenceCombobox;
class ElevationInterfaceGrScene;
class ElevationInterfaceGrView;

class ElevationInterfaceWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    ElevationInterfaceWidget(QWidget *parent = 0);

    void sendActiveStatusToScene() override;

public slots:

    void changeDrawingFrame(const QString &fl);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:

    ReferenceCombobox *frameCombobox;
    QToolButton *printButton;

    ElevationInterfaceGrScene *elevationGrScene;
    ElevationInterfaceGrView *elevationGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};

} // namespace post3dapp
#endif
