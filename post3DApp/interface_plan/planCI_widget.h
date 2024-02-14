#ifndef PLANCI_WIDGET_H
#define PLANCI_WIDGET_H


#include "customdraw_widget.h"
class QToolButton;

namespace post3dapp{
enum class DATATYPE;
class ReferenceCombobox;
class PlanCopyInterfaceGrScene;
class PlanCopyInterfaceGrView;
class UnifiedInputData;

class PlanCopyInterfaceWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    PlanCopyInterfaceWidget(UnifiedInputData*, QWidget *parent = 0);

    void sendActiveStatusToScene() override;

public slots:

    void changeDrawingFloor(const QString &fl);
    void updatePlanView(DATATYPE type);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();
protected:
    UnifiedInputData* copy;
    ReferenceCombobox *floorCombobox;
    QToolButton *printButton;

    PlanCopyInterfaceGrScene *planGrScene;
    PlanCopyInterfaceGrView *planGrView;

    CustomDrawGrView* getView() const override;

    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};

} // namespace post3dapp



#endif // PLANCI_WIDGET_H
