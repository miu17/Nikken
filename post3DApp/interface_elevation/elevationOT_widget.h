#ifndef ELEVATIONOUTDRAWWIDGET_H
#define ELEVATIONOUTDRAWWIDGET_H

#include "customdraw_widget.h"


class QComboBox;
class QLabel;
class QToolButton;
class QSpinBox;

namespace post3dapp
{
enum class DATATYPE;
enum class LISTTYPE;
class ElevationOutputGrScene;
class ElevationOutputGrView;
class ReferenceCombobox;

class ElevationOutputWidget : public CustomDrawWidget
{
    Q_OBJECT

public:
    ElevationOutputWidget(QWidget *parent = nullptr);

public slots:

    void resetCaseCombobox(int);
    void changeDrawingFrame(const QString &fl);
    void changeOutputCase();
    void changeOutputCaseB();
    void updateElevationView(DATATYPE type);
    void resetSceneView();
    void initializePlanView();
    void slotClickedPrintButton();

protected:
    ReferenceCombobox *frameCombobox;

    LISTTYPE listtype;
    QComboBox *outTypeCombobox;
    QComboBox *outCaseACombobox;
    QComboBox *outCaseBCombobox;
    QComboBox *outCaseCCombobox;
    QSpinBox *stepSpinBox;
    QLabel *maxStepLabel;

    //    QToolButton *controlButton;
    QToolButton *printButton;

    ElevationOutputGrScene *elevationGrScene;
    ElevationOutputGrView *elevationGrView;

    CustomDrawGrView *getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;
};

} // namespace post3dapp
#endif
