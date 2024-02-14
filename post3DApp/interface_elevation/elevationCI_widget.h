#ifndef ELEVATIONCIWIDGET_H
#define ELEVATIONCIWIDGET_H

#include "customdraw_widget.h"

class QToolButton;

namespace post3dapp{
class ElevationCopyInterfaceGrScene;
class ElevationCopyInterfaceGrView;
class ReferenceCombobox;
class UnifiedInputData;

class ElevationCopyInterfaceWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    ElevationCopyInterfaceWidget(UnifiedInputData*, QWidget *parent = 0);

    void sendActiveStatusToScene() override;

public slots:

    void changeDrawingFrame(const QString &fl);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:
    UnifiedInputData* copy;
    ReferenceCombobox *frameCombobox;
    QToolButton *printButton;

    ElevationCopyInterfaceGrScene *elevationGrScene;
    ElevationCopyInterfaceGrView *elevationGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};

} // namespace post3dapp

#endif // ELEVATIONCIWIDGET_H
