#ifndef ELEVATIONCL_WIDGET_H
#define ELEVATIONCL_WIDGET_H


#include "customdraw_widget.h"
class QComboBox;
class QToolButton;

namespace post3dapp{
class UnifiedInputData;
class ElevationCalcLoadGrScene;
class ElevationCalcLoadGrView;
class ReferenceCombobox;

class ElevationCalcLoadWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    ElevationCalcLoadWidget(UnifiedInputData*, QWidget *parent = nullptr);

public slots:

    void resetCaseCombobox(int);
    void changeDrawingFrame(const QString &fl);
    void changeOutputCase();
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:
    UnifiedInputData* copy;
    ReferenceCombobox *frameCombobox;

    QComboBox *outCaseACombobox;
    QComboBox *outCaseBCombobox;
    QComboBox *outCaseCCombobox;

//    QToolButton *controlButton;
    QToolButton *printButton;

    ElevationCalcLoadGrScene *elevationGrScene;
    ElevationCalcLoadGrView *elevationGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;
};

} //namespace post3dapp


#endif // ELEVATIONCL_WIDGET_H
