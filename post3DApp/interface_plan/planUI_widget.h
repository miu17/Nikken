#ifndef PLANUIDRAWWIDGET_H
#define PLANUIDRAWWIDGET_H

#include "customdraw_widget.h"
class QToolButton;

namespace post3dapp{
class CustomDrawGrView;
class PlanInterfaceGrScene;
class PlanInterfaceGrView;
class ReferenceCombobox;

class PlanInterfaceWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    PlanInterfaceWidget(QWidget *parent = 0);

    void sendActiveStatusToScene() override;

public slots:

    void changeDrawingFloor(const QString &fl);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();
protected:

    ReferenceCombobox *floorCombobox;
    QToolButton *printButton;

    PlanInterfaceGrScene *planGrScene;
    PlanInterfaceGrView *planGrView;

    CustomDrawGrView* getView() const override;;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};

} // namespace post3dapp



#endif
