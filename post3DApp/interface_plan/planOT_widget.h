#ifndef PLANOUTDRAWWIDGET_H
#define PLANOUTDRAWWIDGET_H

#include "customdraw_widget.h"

class QToolButton;

namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class PlanOutputGrScene;
class PlanOutputGrView;
class ReferenceCombobox;

class PlanOutputWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    PlanOutputWidget(QWidget *parent = 0);

public slots:

    void resetCaseCombobox(int);
    void changeDrawingFloor(const QString &fl);
    void changeOutputCaseB();
    void changeOutputCase();
    void updatePlanView(DATATYPE type);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:

    ReferenceCombobox *floorCombobox;

    QComboBox *outTypeCombobox;
    QComboBox *outCaseACombobox;
    QComboBox *outCaseBCombobox;
    QComboBox *outCaseCCombobox;
    QComboBox *outCaseDCombobox;
    QComboBox *outCaseECombobox;
    QSpinBox *stepSpinBox;
    QLabel *maxStepLabel;

//    QToolButton *controlButton;
    QToolButton *printButton;

    PlanOutputGrScene *planGrScene;
    PlanOutputGrView *planGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};
} // namespace post3dapp

#endif
