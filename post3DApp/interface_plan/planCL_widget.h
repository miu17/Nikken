#ifndef PLANCL_WIDGET_H
#define PLANCL_WIDGET_H

#include "customdraw_widget.h"

class QToolButton;

namespace post3dapp{
enum class DATATYPE;

class UnifiedInputData;
class ReferenceCombobox;
class PlanCalcLoadGrScene;
class PlanCalcLoadGrView;

class PlanCalcLoadWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    PlanCalcLoadWidget(UnifiedInputData*, QWidget *parent = nullptr);

public slots:

    void resetCaseCombobox(int);
    void changeDrawingFloor(const QString &fl);
    void changeOutputCase();
    void updatePlanView(DATATYPE type);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:
    UnifiedInputData* copy;
    ReferenceCombobox *floorCombobox;

    QComboBox *outCaseACombobox;
    QComboBox *outCaseBCombobox;
    QComboBox *outCaseCCombobox;

//    QToolButton *controlButton;
    QToolButton *printButton;

    PlanCalcLoadGrScene *planGrScene;
    PlanCalcLoadGrView *planGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};
} // namespace post3dapp
#endif // PLANCL_WIDGET_H
